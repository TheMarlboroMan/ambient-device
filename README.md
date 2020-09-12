##Resources:

###Fonts:
	Akashi, by Ed Merritt.
	Alcubierre, by Matt Ellis.

###Pictures:

	Default ones, all by myself.

##Adding your own pictures

TODO

##Troubleshooting:

- Q: Some of my pictures are just a gray blob!
- A: Your computer has not enough juice to runo that picture. Try scaling it to a lower resolution... More specifically, pictures are loaded into OpenGL textures (that's the code I have at hand) and I have personally experienced my computer not being able to handle some textures.

- Q: Pictures appear for a few seconds, the dissapear and are replaced by a black screen or even a picture that appeared not long ago... Also, sometimes the clock seems to go backwards.
- A: Disable "lazy_render" in the configuration file. Lazy rendering is supposed to update the screen only when needed (a picture change, a clock change...) but that does not play nice with all systems. Disabling it will force your computer to draw non-stop, but at least it won't be annoying.

- Q: My picture is sideways!!
- A: The program does nothing with EXIF data (which stores the way the camera was oriented). Please, rotate it manually.

## TODO

- segmentation violation when showing seconds
- crash when loading invalid pic, better to skip and detect infinite bad loop.
- config menu
- pic crossfade (optional)
- update source to new version of fw
- music?
- pic service?
- check in some other systems for opengl glitches
- runnable from everywhere
- skip pic, prev pic, pause slide, restore slide...
- preload next pic in separate thread
- pic count (N/M)
