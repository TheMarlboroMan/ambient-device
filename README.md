##Resources:

###Fonts:
	Akashi, by Ed Merritt.
	Alcubierre, by Matt Ellis.

###Pictures:

	Default ones, all by myself.

##Adding your own pictures

I would like to make this a bit better, but don't really have time now. It is really easy tough, just putting the files in a directory and modifying an index file. If you checked the code and could not figure it out just drop me a note and I will set better instructions up.

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
- config menu <- The "refactor" branch should take care of this.
- pic crossfade (optional)
- update source to new version of fw <-- There is actually a "refactor" branch for this!
- music?
- pic service?
- check in some other systems for opengl glitches
- runnable from everywhere <-- Pretty sure the "refactor" branch does this.
- skip pic, prev pic, pause slide, restore slide...
- preload next pic in separate thread
- pic count (N/M) <-- I think the "refactor" branch takes care of this.
