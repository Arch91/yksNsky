#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <dirent.h>

int main(void) {
	DIR *dir = opendir(".");
	if (dir == NULL) {
		perror("Folder opening error");
		return 1;
	}

	struct dirent *entry;
	int processed_count = 0;

	while ((entry = readdir(dir)) != NULL) {
		size_t len = strlen(entry->d_name);
		uint32_t audio_sig = 0, sub_sig = 0;
		
		// Check .fuz files only
		if (len > 4 && strcasecmp(entry->d_name + len - 4, ".fuz") == 0) {
			
			FILE *fuz = fopen(entry->d_name, "rb");
			if (!fuz) {
				fprintf(stderr, "Could not open file: %s\n", entry->d_name);
				continue;
			}

			// Jump to 8th byte
			if (fseek(fuz, 8, SEEK_SET) != 0) {
				fclose(fuz);
				continue;
			}

			uint32_t lip_length = 0;
			if (fread(&lip_length, 1, 4, fuz) != 4) {
				fclose(fuz);
				continue;
			}

			int xwma_type = 0; // 0 - unk, 1 - XWMA, 2 - MSFC
			
			// Audiofile position
			long audio_start_pos = 12 + lip_length;
			
			if (fseek(fuz, audio_start_pos, SEEK_SET) == 0) {
				if (fread(&audio_sig, 1, 4, fuz) == 4) {
					if (audio_sig == 0x46464952) { // RIFF
						// Jump to 4 bytes further from RIFF (audio_start_pos + 4)
						if (fseek(fuz, 4, SEEK_CUR) == 0) {
							if (fread(&sub_sig, 1, 4, fuz) == 4) {
								if (sub_sig == 0x414D5758) { // XWMA
									xwma_type = 1;
								}
							}
						}
					} 
					else if (audio_sig == 0x4346534D) { // MSFC
						xwma_type = 2;
					}
				}
			}

			if (xwma_type == 0) {
				fprintf(stderr, "Unknown content of .fuz file: %s\n", entry->d_name);
				fclose(fuz);
				continue;
			}

			// Go back to 12th byte
			if (fseek(fuz, 12, SEEK_SET) != 0) {
				fclose(fuz);
				continue;
			}

			char lip_name[256];
			if (len - 4 >= sizeof(lip_name) - 5) {
				fclose(fuz);
				continue;
			}
			strncpy(lip_name, entry->d_name, len - 4);
			lip_name[len - 4] = '\0';
			strcat(lip_name, ".lip");

			int lip_success = 0;

			if (lip_length > 0) {
				uint8_t *buffer = malloc(lip_length);
				if (!buffer) {
					fprintf(stderr, "Memory allocating error for %s\n", entry->d_name);
					fclose(fuz);
					continue;
				}

				if (fread(buffer, 1, lip_length, fuz) == lip_length) {
					FILE *lip = fopen(lip_name, "wb");
					if (lip) {
						fwrite(buffer, 1, lip_length, lip);
						fclose(lip);
						lip_success = 1;
					}
				}
				free(buffer);
			} else {
				FILE *lip = fopen(lip_name, "wb");
				if (lip) {
					fclose(lip);
					lip_success = 1;
				}
			}

			if (lip_success) {
				// Jump to the end of this .fuz file to obtain it's total size
				if (fseek(fuz, 0, SEEK_END) == 0) {
					long total_size = ftell(fuz);
					long audio_size = total_size - audio_start_pos;

					if (audio_size > 0) {
						// Go back to the audiofile start position
						fseek(fuz, audio_start_pos, SEEK_SET);

						char audio_name[256];
						strncpy(audio_name, entry->d_name, len - 4);
						audio_name[len - 4] = '\0';
						
						char audio_ext[5] = "";
						if (xwma_type == 1) {
							strcpy(audio_ext, ".xwm");
                        } else if (xwma_type == 2) {
							strcpy(audio_ext, ".msf");
						}
						strcat(audio_name, audio_ext);

						uint8_t *audio_buffer = malloc(audio_size);
						if (audio_buffer) {
							if (fread(audio_buffer, 1, audio_size, fuz) == audio_size) {
								FILE *audio_file = fopen(audio_name, "wb");
								if (audio_file) {
									fwrite(audio_buffer, 1, audio_size, audio_file);
									fclose(audio_file);
									
									printf("%s -> %s , %s\n", entry->d_name, lip_name, audio_name);
								} else {
									fprintf(stderr, "Could not create audio file: %s\n", audio_name);
								}
							} else {
								fprintf(stderr, "Audio data reading error from %s\n", entry->d_name);
							}
							free(audio_buffer);
						} else {
							fprintf(stderr, "Memory allocation error for audio in %s\n", entry->d_name);
						}
					}
				}
			}

			fclose(fuz);
		}
	}

	closedir(dir);
	return 0;
}
