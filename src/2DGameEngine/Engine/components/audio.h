#ifndef __AUDIO_H__
#define __AUDIO_H__
#include "base_component.h"
typedef struct Audio
{
	Component super;
	char *filePath;
	bool playing;
	bool loop;
	int volume;

	void (*play)(struct Audio *);
	void (*pause)(struct Audio *);
	void (*stop)(struct Audio *);
	void (*setVolume)(struct Audio *, int);
	char *(*getFilePath)(struct Audio *);
	char *(*getMeta)(struct Component *);
	void (*setMeta)(struct Component *, char *meta);
} Audio;

Audio *newAudio(char *filePath, bool loop);
void destoryAudio(struct Audio *audio);

#endif