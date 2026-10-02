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

	while ((entry = readdir(dir)) != NULL) {
		size_t len = strlen(entry->d_name);
		size_t ext_len = 4; // Extension length (.xwm or .msf)

		if (len > 4) {
			if (strcasecmp(entry->d_name + len - 4, ".xwm") == 0 ||
				strcasecmp(entry->d_name + len - 4, ".msf") == 0) {
				// Extracting file name
				char base_name[512];
				if (len - ext_len >= sizeof(base_name)) {
					continue;
				}
				strncpy(base_name, entry->d_name, len - ext_len);
				base_name[len - ext_len] = '\0';

				char lip_name[512];
				snprintf(lip_name, sizeof(lip_name), "%s.lip", base_name);

				FILE *lip = fopen(lip_name, "rb");
				if (!lip) {
					fprintf(stderr, "%s found, but paired %s is missing\n", entry->d_name, lip_name);
					continue;
				}

				// Measuring the length of .lip file
				if (fseek(lip, 0, SEEK_END) != 0) {
					fclose(lip);
					continue;
				}
				long lip_size = ftell(lip);
				uint32_t lip_length = (uint32_t)lip_size;
				rewind(lip);

				FILE *audio = fopen(entry->d_name, "rb");
				if (!audio) {
					fprintf(stderr, "Could not open audio file: %s\n", entry->d_name);
					fclose(lip);
					continue;
				}

				// Measuring the length of audiofile
				if (fseek(audio, 0, SEEK_END) != 0) {
					fclose(audio);
					fclose(lip);
					continue;
				}
				long audio_size = ftell(audio);
				rewind(audio);

				char fuz_name[512];
				snprintf(fuz_name, sizeof(fuz_name), "%s.fuz", base_name);

				FILE *fuz = fopen(fuz_name, "wb");
				if (!fuz) {
					fprintf(stderr, "Could not create file: %s\n", fuz_name);
					fclose(audio);
					fclose(lip);
					continue;
				}

				// FUZ header
				uint8_t header[8] = {0x46, 0x55, 0x5A, 0x45, 0x01, 0x00, 0x00, 0x00};
				fwrite(header, 1, 8, fuz);

				// 4 bytes of .lip file length
				fwrite(&lip_length, 1, 4, fuz);

				if (lip_length > 0) {
					uint8_t *lip_buffer = malloc(lip_length);
					if (lip_buffer) {
						if (fread(lip_buffer, 1, lip_length, lip) == lip_length) {
							fwrite(lip_buffer, 1, lip_length, fuz);
						}
						free(lip_buffer);
					}
				}

				// audiofile
				if (audio_size > 0) {
					uint8_t *audio_buffer = malloc(audio_size);
					if (audio_buffer) {
						if (fread(audio_buffer, 1, audio_size, audio) == audio_size) {
							fwrite(audio_buffer, 1, audio_size, fuz);
						}
						free(audio_buffer);
					}
				}

				fclose(fuz);
				fclose(audio);
				fclose(lip);

				printf("%s , %s -> %s\n", entry->d_name, lip_name, fuz_name);
			}
		}
	}

	closedir(dir);
	return 0;
}
