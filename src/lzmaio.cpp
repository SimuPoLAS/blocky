#include "lzmaio.hpp"

//static LZMAFILE* lzmaopen(const char* filepath, const char* mode) {
//	LZMAFILE* file = (LZMAFILE*)malloc(sizeof(LZMAFILE));
//	file->decode = LZMA_STREAM_INIT;
//	file->encode = LZMA_STREAM_INIT;
//	// TODO: this has to be done better
//	// in this case, only reading is possible
//	// but writing (encoding) should also be possible
//	// NOTE: maybe make 2 lzma_streams? => now using 2 lzma_streams
//	if (lzma_stream_decoder(&file->decode, 99 * 1024 * 1024, LZMA_TELL_NO_CHECK) != LZMA_OK)
//		return NULL;
//	if (lzma_easy_encoder(&file->encode, 6, LZMA_CHECK_CRC32) != LZMA_OK)
//		return NULL;
//
//	file->fp = fopen(filepath, mode);
//	file->in_size = 0;
//	file->out_size = 0;
//	return file;
//}
//
//static int lzmaclose(LZMAFILE* file) {
//	for (;;) {
//		lzma_ret ret = lzma_code(&file->encode, LZMA_FINISH);
//		file->out_size = file->BUFFER_SIZE - file->encode.avail_out;
//
//		if (file->encode.avail_out == 0 || ret == LZMA_STREAM_END) {
//			fwrite(file->out, sizeof(uint8_t), file->out_size, file->fp);
//
//			file->encode.next_out = file->out;
//			file->encode.avail_out = file->BUFFER_SIZE;
//			file->out_size = 0;
//		}
//
//		if (ret == LZMA_STREAM_END) break;
//
//		if (ret != LZMA_OK) return -1;
//	}
//
//	lzma_end(&file->decode);
//	lzma_end(&file->encode);
//	fclose(file->fp);
//	free(file);
//}

//static size_t lzmaread(void* buffer, size_t size, size_t count, LZMAFILE* file) {
//	file->decode.next_out = (uint8_t*)buffer;
//	file->decode.avail_out = size * count;
//
//	while (file->decode.avail_out > 0) {
//		if (file->in_size == 0) {
//			size_t read = fread(file->in, sizeof(char), file->BUFFER_SIZE, file->fp);
//			file->in_size = read;
//		}
//
//		file->decode.next_in = file->in;
//		file->decode.avail_in = file->in_size;
//
//		lzma_ret rc = lzma_code(&file->decode, LZMA_RUN);
//
//		if (rc == LZMA_STREAM_END)
//			break;
//
//		if (rc != LZMA_OK)
//			return -1;
//
//		if (file->decode.avail_in > 0) {
//			// copy the rest from avail_in to BUFFER_SIZE to the beginning of in buffer
//			char tmp[4096];
//			memcpy(tmp, file->decode.next_in, file->decode.avail_in);
//			memcpy(file->in, tmp, file->decode.avail_in);
//		}
//		file->in_size = file->decode.avail_in;
//	}
//
//	return size*count;
//}

//static size_t lzmawrite(const void* buffer, size_t size, size_t count, LZMAFILE* file) {
//	file->encode.next_in = (uint8_t*)buffer;
//	file->encode.avail_in = size * count;
//
//	file->encode.next_out = file->out + file->out_size;
//	file->encode.avail_out = file->BUFFER_SIZE - file->out_size;
//
//	while (file->encode.avail_in > 0) {
//		lzma_ret ret = lzma_code(&file->encode, LZMA_RUN);
//		file->out_size = file->BUFFER_SIZE - file->encode.avail_out;
//
//		if (file->encode.avail_out == 0 || ret == LZMA_STREAM_END) {
//			fwrite(file->out, sizeof(uint8_t), file->BUFFER_SIZE, file->fp);
//
//			file->encode.next_out = file->out;
//			file->encode.avail_out = file->BUFFER_SIZE;
//		}
//
//		if (ret == LZMA_STREAM_END) break;
//
//		if (ret == LZMA_OK) continue;
//
//		return -1;
//	}
//
//	return size * count;
//}