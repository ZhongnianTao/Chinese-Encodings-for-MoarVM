#include "moar.h"
#include "gb2312_codeindex.h"

MVMString * MVM_string_gb2312_decode(MVMThreadContext *tc, const MVMObject *result_type, const char *gb2312, size_t bytes) {
	MVMString *result = (MVMString *)REPR(result_type)->allocate(tc, STABLE(result_type));
	
	result->body.storage_type = MVM_STRING_GRAPHEME_32;
	result->body.storage.blob_32 = MVM_malloc(sizeof(MVMGrapheme32) * bytes);
	
	size_t i, result_graphs;
	
	result_graphs = 0;
	
	for (i = 0; i < bytes; i++) {
		if (0 <= gb2312[i] && gb2312[i] <= 127) {
			//	ASCII character
			if (gb2312[i] == '\r' && i+1 < bytes && gb2312[i+1] == '\n') {
				result->body.storage.blog_32[result_graphs++] = MVM_nfg_crlf_grapheme(tc);
				i++;
			}
			else {
				result->body.storage.blob_32[result_graphs++] = gb2312[i];
			}
		}
		else {
			if (i+1 < bytes && (gb2312[i+1] > 127 || gb2312[i+1] < 0)) {
				MVMuint8 byte1 = gb2312[i];
				MVMuint8 byte2 = gb2312[i+1];
				MVMuint16 codepoint = (MVMuint16)byte1 * 256 + byte2;
				MVMGrapheme32 index = gb2312_cp_to_index(codepoint);
				if (index != GB2312_NULL) {
					result->body.storage.blob_32[result_graphs++] = index;
					i++;
				}
				else {
					MVM_exception_throw_adhoc(tc, "Error decoding gb2312 string: could not decode codepoint 0x%hhX", codepoint);
				}
			}
			else {
				MVM_exception_throw_adhoc(tc, "Error decoding gb2312 string: invalid gb2312 format (two bytes for a gb2312 character)");
			}
		}
	}
	
	result->body.num_graphs = result_graphs;
	
	return result;
}

MVMuint32 MVM_string_gb2312_decodestream(MVMThreadContext *tc, MVMDecodeStream *ds, 
					const MVMint32 *stopper_chars, MVMDecodeStreamSeparators *seps) {
	MVMint32 count = 0, total = 0;
	MVMint32 bufsize;
	MVMGrapheme32 *buffer = NULL;
	MVMDecodeStreamBytes *cur_bytes = NULL;
	MVMDecodeStreamBytes *last_accept_bytes = ds->bytes_head;
	MVMint32 last_accept_pos, last_was_cr;
	MVMuint32 reached_stopper;
	
	MVMint32 last_was_first_byte;
	MVMint32 last_codepoint;
	
	if (!ds->bytes_head)
		return 0;
	last_accept_pos = ds->bytes_head_pos;
	
	if (stopper_chars && *stopper_chars == 0)
		return 1;
	
	bufsize = ds->result_size_guess;
	buffer = MVM_malloc(bufsize * sizeof(MVMGrapheme32));
	
	cur_bytes = ds->bytes_head;
	last_was_cr = 0;
	reached_stopper = 0;
	
	last_was_first_byte = 0;
	last_codepoint = 0;
	
	while (cur_bytes) {
		MVMint32 pos = cur_bytes == ds->bytes_head ? ds->bytes_head_pos : 0;
		MVMuint8 *bytes = (MVMuint8 *)cur_bytes->bytes;
		
		while (pos < cur_bytes->length) {
			MVMGrapheme32 graph;
			MVMuint16 codepoint = bytes[pos++];
		
			if (codepoint <= 127) {
				if (last_was_first_byte) {
					MVM_exception_throw_adhoc(tc, "Error decoding gb2312 string: invalid gb2312 format (two bytes for a gb2312 character)");
				}
			
				if (last_was_cr) {
					if (codepoint == '\n') {
					    graph = MVM_unicode_normalizer_translated_crlf(tc, &(ds->norm));
					}
					else {
					    graph = '\r';
					    pos--;
					}
					last_was_cr = 0;
				}
				else if (graph == '\r') {
					last_was_cr = 1;
					continue;
				}
				else {
					graph = codepoint;
				}
			}
			else {
				if (last_was_first_byte) {
					MVMuint16 combined_codepoint = last_codepoint * 256 + codepoint;
					graph = gb2312_cp_to_index(combined_codepoint);
					if (graph == GB2312_NULL) {
						MVM_exception_throw_adhoc(tc, "Error decoding gb2312 string: could not decode codepoint 0x%hhX", combined_codepoint);
					}
					last_was_first_byte = 0;
				}
				else {
					last_was_first_byte = 1;
					last_codepoint = codepoint;
					continue;
				}
			}
		
			if (count == bufsize) {
				MVM_string_decodestream_add_chars(tc, ds, buffer, bufsize);
				buffer = MVM_malloc(bufsize * sizeof(MVMGrapheme32));
				count = 0;
			}
		
			buffer[count++] = graph;
			last_accept_bytes = cur_bytes;
			last_accept_pos = pos;
			total++;
		
			if (MVM_string_decode_stream_maybe_sep(tc, seps, codepoint) ||
				(stopper_chars && *stopper_chars == total)) {
				reached_stopper = 1;
				goto done;
			}
		}
		
		cur_bytes = cur_bytes -> next;
	}
	
done:
	
	if (count) {
		MVM_string_decodestream_add_chars(tc, ds, buffer, count);
	}
	else {
		MVM_free(buffer);
	}
	MVM_string_decodestream_discard_to(tc, ds, last_accept_bytes, last_accept_pos);
	
    return reached_stopper;
}

