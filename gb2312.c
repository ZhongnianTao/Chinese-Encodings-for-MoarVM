#include "moar.h"
#include "gb2312_codeindex.h"

MVMString * MVM_string_gb2312_decode(MVMThreadContext *tc, const MVMObject *result_type, const char *gb2312, size_t bytes)
{
	MVMString *result = (MVMString *)REPR(result_type)->allocate(tc, STABLE(result_type));
	
	result->body.storage_type = MVM_STRING_GRAPHEME_32;
	result->body.storage.blob_32 = MVM_malloc(sizeof(MVMGrapheme32) * bytes);
	
	size_t i, result_graphs;
	
	result_graphs = 0;
	
	for (i = 0; i < bytes; i++)
	{
		if (0 <= gb2312[i] && gb2312[i] <= 127)
		{
			//	ASCII character
			result->body.storage.blob_32[result_graphs++] = gb2312[i];
		}
		else
		{
			if (i+1 < bytes && (gb2312[i+1] > 127 || gb2312[i+1] < 0))
			{
				MVMuint8 byte1 = gb2312[i];
				MVMuint8 byte2 = gb2312[i+1];
				MVMuint16 codepoint = (MVMuint16)byte1 * 256 + byte2;
				MVMGrapheme32 index = gb2312_cp_to_index(codepoint);
				if (index != GB2312_NULL)
				{
					result->body.storage.blob_32[result_graphs++] = index;
				}
				else
				{
					MVM_exception_throw_adhoc(tc, "Error decoding gb2312 string: could not decode codepoint 0x%hhX", codepoint);
				}
			}
			else
			{
				MVM_exception_throw_adhoc(tc, "Error decoding gb2312 string: invalid gb2312 format (two bytes for a gb2312 character)");
			}
		}
	}
	
	result->body.num_graphs = result_graphs;
	
	return result;
}

