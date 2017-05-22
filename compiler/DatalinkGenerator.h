/*!
mana (compiler)

@file	DatalinkGenerator.h
@brief	データセクションに関するヘッダファイル
@detail	このファイルはデータセクションに関係するヘッダファイルです。
@author	Shun Moriya
@date	2003-
*/

#include <string>
#include <vector>

namespace mana
{
	/*!
	データセクション生成クラス
	*/
	class DatalinkGenerator
	{
	private:
		//! データエントリの型
		struct Entry
		{
			std::string mFilename;
			size_t mFilesize;
			size_t mPaddingSize;
		};

		//! データエントリ
		std::vector<Entry> mEntites;

	public:
		/*!
		コンストラクタ
		*/
		DatalinkGenerator() {}

		/*!
		デストラクタ
		*/
		virtual ~DatalinkGenerator() = default;

		size_t GetSize() const
		{
			return mEntites.size();
		}

		int32_t Append(const char* filename)
		{
#if 0
			FILE* file;

			Entry entry;
			entry.mFilename = filename;
			entry.mFilesize;
			entry.mPaddingSize;



			size_t alignment_size = sizeof(void*);
			size_t string_length;

			if (mFilename == NULL)
			{
				CompileError("file name not found");
				return -1;
			}

			string_length = strlen(mFilename) + 1;

			entry = mana_calloc(sizeof(Entry), 1);
			entry->mFilename = mana_malloc(string_length);

#if defined(__STDC_WANT_SECURE_LIB__)
			strcpy_s(entry->mFilename, string_length, mFilename);
#else
			strcpy(entry->mFilename, mFilename);
#endif

#if defined(__STDC_WANT_SECURE_LIB__)
			if (fopen_s(&file, mFilename, "rb") != 0)
#else
			if ((file = fopen(mFilename, "rb")) == NULL)
#endif
			{
				mana_free(entry->mFilename);
				CompileError("unable to open '%s'", mFilename);
				return -1;
			}
			else
			{
				Entry* last_entry;
				size_t index;

				fseek(file, 0L, SEEK_END);
				entry->mFilesize = ftell(file);
				entry->mPaddingSize = ((entry->mFilesize + (alignment_size - 1)) / alignment_size * alignment_size) - entry->mFilesize;
				entry->next = NULL;
				fclose(file);

				if (alignment_size != MANA_DATALINK_ALIGNMENT_SIZE)
				{
					last_entry = datalink_generator_get_last_entry();
					if (last_entry)
					{
						last_entry->mPaddingSize = ((last_entry->mFilesize + (alignment_size - 1)) / alignment_size * alignment_size) - last_entry->mFilesize;
					}
				}
				last_entry = datalink_generator_get_last_entry();
				if (last_entry)
					last_entry->next = entry;
				else
					instance = entry;

				index = mNumberOfFiles;
				mNumberOfFiles++;
				return (int32_t)index;
			}
#endif
		}

		bool datalink_generator_write_data(mana_stream* stream)
		{
#if 0
			Entry* entry;
			mana_datalink_file_header header;
			size_t mFilesize;
			size_t header_size;

			srand((unsigned)time(NULL));

			header.total_data_size = GetSize();
			header.number_of_datas = GetSize();
			mana_stream_push_data(stream, &header, sizeof(header));
			/*
			{
			CompileError("file write failed");
			return false;
			}
			*/

			mFilesize = 0;
			for (entry = instance; entry != NULL; entry = entry->next)
			{
				mana_datalink_info_header entry_header;

				entry_header.offset = mFilesize;
				entry_header.size = entry->mFilesize;

				mana_stream_push_data(stream, &entry_header, sizeof(entry_header));
				/*
				if(fwrite(&entry_header, sizeof(entry_header), 1, file) != 1)
				{
				CompileError("file write failed");
				return false;
				}
				*/

				mFilesize += entry->mFilesize + entry->mPaddingSize;
			}

			header_size = (GetSize() * sizeof(mana_datalink_info_header)
				+ sizeof(mana_datalink_file_header)) % MANA_DATALINK_ALIGNMENT_SIZE;
			if (header_size > 0)
			{
				size_t i;

				header_size = MANA_DATALINK_ALIGNMENT_SIZE - header_size;
				for (i = 0; i < header_size; i++)
					mana_stream_push_unsigned_char(stream, (uint8_t)rand());
			}

			for (entry = instance; entry != NULL; entry = entry->next)
			{
				FILE* in;

#if defined(__STDC_WANT_SECURE_LIB__)
				if (fopen_s(&in, entry->mFilename, "rb") != 0)
#else
				if ((in = fopen(entry->mFilename, "rb")) == NULL)
#endif
				{
					CompileError("unable to open '%s'", entry->mFilename);
					return false;
				}

				void* program = mana_malloc(entry->mFilesize);
				if (program == NULL)
				{
					fclose(in);
					printf("memory allocation failed\n");
					return false;
				}

				if (fread(program, 1, entry->mFilesize, in) != entry->mFilesize)
				{
					fclose(in);
					free(program);
					printf("file read failed: %s\n", entry->mFilename);
					return false;
				}

				fclose(in);

				mana_stream_push_data(stream, program, entry->mFilesize);

				free(program);

				for (size_t i = 0; i < entry->mPaddingSize; i++)
					mana_stream_push_unsigned_char(stream, (uint8_t)rand());

				fclose(in);
			}
#endif
			return true;
		}
	};
}
