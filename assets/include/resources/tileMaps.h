
//{{BLOCK(tileMaps)

//======================================================================
//
//	tileMaps, 1024x1024@4, 
//	+ palette 256 entries, not compressed
//	+ 319 tiles (t|f|p reduced) lz77 compressed
//	+ regular map (flat), not compressed, 128x128 
//	Total size: 512 + 5936 + 32768 = 39216
//
//	Time-stamp: 2022-12-30, 23:12:17
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_TILEMAPS_H
#define GRIT_TILEMAPS_H

#define tileMapsTilesLen 5936
extern const unsigned int tileMapsTiles[1484];

#define tileMapsMapLen 32768
extern const unsigned short tileMapsMap[16384];

#define tileMapsPalLen 512
extern const unsigned short tileMapsPal[256];

#endif // GRIT_TILEMAPS_H

//}}BLOCK(tileMaps)
