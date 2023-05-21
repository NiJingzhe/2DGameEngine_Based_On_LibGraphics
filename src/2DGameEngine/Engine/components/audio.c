#include "audio.h"
#include "base_component.h"

static void initAudio(struct Audio *audio, char *filePath, bool loop);
static void renderAudio(Component *c);
static void updateAudio(Component *c, ...);
static const int returnAudioType(ComponentNode c);
static void Audio_play(struct Audio *audio);
static void Audio_pause(struct Audio *audio);
static void Audio_stop(struct Audio *audio);
static void Audio_setVolume(struct Audio *audio, int volume);
static char *Audio_getFilePath(struct Audio *audio);



Audio *newAudio(char *filePath, bool loop)
{
	Audio *audio = (Audio *)malloc(sizeof(Audio));
	initAudio(audio, filePath, loop);
	return audio;
}

static void initAudio(struct Audio *audio, char *filePath, bool loop)
{

	Component *super = newComponent(renderAudio, updateAudio);
	memcpy(&(audio->super), super, sizeof(Component));
	audio->super.vptr = NULL;
	audio->super.vptr = (componentvTable *)calloc(1, sizeof(componentvTable));
#if MEM_DEBUG
	MEM_BLOCK_NUM++;
	printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
#endif
	if (audio->super.vptr == NULL)
	{
		printf("Cannot allocate memory for vptr of audio component\n");
		return;
	}
	memcpy(audio->super.vptr, super->vptr, sizeof(componentvTable));
	destoryComponent(super);
	audio->super.vptr->getComponentType = returnAudioType;
	audio->super.meta = NULL;
	audio->super.next = NULL;
	audio->super.prev = NULL;

	audio->filePath = filePath;
	audio->playing = FALSE;
	audio->loop = loop;
	audio->volume = 100;
	audio->play = Audio_play;
	audio->pause = Audio_pause;
	audio->stop = Audio_stop;
	audio->setVolume = Audio_setVolume;
	audio->getFilePath = Audio_getFilePath;
	audio->getMeta = audio->super.getMeta;
	audio->setMeta = audio->super.setMeta;

	char command[256];
	sprintf(command, "open \"%s\" type mpegvideo alias audio%d", audio->filePath, (int)audio);
	mciSendString(command, NULL, 0, NULL);
}

static void renderAudio(Component *c)
{
	return;
}

static void updateAudio(Component *c, ...)
{
	return;
}

static const int returnAudioType(ComponentNode c)
{
	return AUDIO;
}

static void Audio_play(struct Audio *audio)
{
	char command[256];
	if (!audio->loop)
	{
		sprintf(command, "play audio%d", (int)audio);
		mciSendString(command, NULL, 0, NULL);
	}
	else
	{
		sprintf(command, "play audio%d repeat", (int)audio);
		mciSendString(command, NULL, 0, NULL);
	}

	audio->playing = TRUE;
}

static void Audio_pause(struct Audio *audio)
{
	char command[256];
	sprintf(command, "pause audio%d", (int)audio);
	mciSendString(command, NULL, 0, NULL);

	audio->playing = FALSE;
}

static void Audio_stop(struct Audio *audio)
{
	char command[256];
	sprintf(command, "stop audio%d", (int)audio);
	mciSendString(command, NULL, 0, NULL);
	sprintf(command, "seek audio%d to %ld", (int)audio, 0L);
	mciSendString(command, NULL, 0, NULL);

	audio->playing = FALSE;
}

static void Audio_setVolume(struct Audio *audio, int volume)
{
	char command[256];
	sprintf(command, "setaudio audio%d volume to %d", (int)audio, volume);
	mciSendString(command, NULL, 0, NULL);

	audio->volume = volume;
}

static char *Audio_getFilePath(struct Audio *audio)
{
	return audio->filePath;
}

void destoryAudio(struct Audio *audio)
{
	char command[256];
	sprintf(command, "close audio%d", (int)audio);
	mciSendString(command, NULL, 0, NULL);
	free(audio->super.vptr);
#if MEM_DEBUG
	MEM_BLOCK_NUM--;
	printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
#endif
	free(audio);
#if MEM_DEBUG
	MEM_BLOCK_NUM--;
	printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
#endif
}