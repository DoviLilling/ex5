#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Song {
    char* title;
    char* artist;
    int year;
    char* lyrics;
    int streams;
} Song;

typedef struct Playlist {
    char* name;
    Song* songs;
    int songsCount;
} Playlist;

#define mainMenuText "Please Choose:\n        1. Watch playlists\n        2. Add playlist\n        3. Remove playlist\n        4. exit\n"
#define playlistMenuText "        1. Show Playlist\n        2. Add Song\n        3. Delete Song\n        4. Sort\n        5. Play\n        6. exit\n"
#define sortPlaylistMenuText "choose:\n1. sort by year\n2. sort by streams - ascending order\n3. sort by streams - descending order\n4. sort alphabetically\n"
#define maxNameSize 1024
#define maxSongsSize 1048576 // 1024*1024


int menu(char* prompt) {
    int choice;
    printf("%s", prompt);
    scanf("%d", &choice);
    return choice;
}


void printWatchPlaylistsMenu(Playlist* playlists, int playlistsCount){
    printf("Choose a playlist:\n");
    for (int i = 0; i < playlistsCount; i++) {
        printf("        %d. %s\n", i + 1, playlists[i].name);
    }
    printf("        %d. Back to main menu\n", playlistsCount + 1);
}


void printPlaylist(Song *songs, int songsCount) {
    for (int i = 0; i < songsCount; i++)
        printf("%d. Title: %s\n   Artist: %s\n   Released: %d\n   Streams: %d\n\n", i+1, songs[i].title, songs[i].artist, songs[i].year, songs[i].streams);
}


void switchSongs(Song* song1, Song* song2) {
    char* tempString;
    int tempInt;

    tempString = song1->title;
    song1->title = song2->title;
    song2->title = tempString;

    tempString = song1->artist;
    song1->artist = song2->artist;
    song2->artist = tempString;

    tempInt = song1->year;
    song1->year = song2->year;
    song2->year = tempInt;

    tempString = song1->lyrics;
    song1->lyrics = song2->lyrics;
    song2->lyrics = tempString;

    tempInt = song1->streams;
    song1->streams = song2->streams;
    song2->streams = tempInt;
}


void freeSong(Song** songs, int* songsCount, int songIndexToFree) {
printf("~~~~~~~ freeSong start ~~~~~~~\n");
printf("Freeing song %d\n", songIndexToFree);
printf("Freeing title %s from %p\n", (*songs)[songIndexToFree].title, (*songs)[songIndexToFree].title);
    free((*songs)[songIndexToFree].title);
printf("Freeing artist %s from %p\n", (*songs)[songIndexToFree].artist, (*songs)[songIndexToFree].artist);
    free((*songs)[songIndexToFree].artist);
printf("Freeing lyrics %s from %p\n", (*songs)[songIndexToFree].lyrics, (*songs)[songIndexToFree].lyrics);
    free((*songs)[songIndexToFree].lyrics);
    // sending the deleted song to the end of the list
printf("Pushing freed song to end of array, verifying...\n");
printf("Current title of last song is %s\n", (*songs)[*songsCount - 1].title);
    for (int i = songIndexToFree; i < *songsCount - 1; i++) {
printf("Pushing...\n");
        switchSongs(&(*songs)[i], &(*songs)[i + 1]);
    }
printf("Title of last song after push is %s\n", (*songs)[*songsCount - 1].title);
printf("Freeing song memory from songs. Reminder, songs count before delete is: %d, songs address is %p\n", *songsCount, *songs);
    if (*songsCount == 1) {
printf("Freeing...\n");
        free(*songs);
printf("Freed.\n");
    }
    else {
        *songs = realloc(*songs, (*songsCount - 1) * sizeof(Song));
printf("Songs reallocated to %p\n", *songs);
    }
    *songsCount = *songsCount - 1;
printf("Songs count now is: %d\n", *songsCount);
printf("Song freed.\n");
printf("~~~~~~~ freeSong end ~~~~~~~\n\n");
}


void deleteSong(Song** songs, int *songsCount) {
    int choice = -1;
    printPlaylist(*songs, *songsCount);
    printf("choose a song to delete, or 0 to quit:\n");
    scanf("%d", &choice);
    if (choice > 0 && choice <= *songsCount)
        freeSong(songs, songsCount, choice - 1);
    printf("Song deleted successfully.\n");
}


void playSong(Song* song) {
    printf("Now playing %s:\n%s\n", song->title, song->lyrics);
    song->streams++;
}


char* allocAndCopyString(char* source) {
    char* target;
    target = malloc(strlen(source) + 1);
    if (target == NULL) exit(1);
    strcpy(target, source);
    return target;
}


Song* allocAndInitSong(Song* songs, int songsCount) {
printf("~~~~~~~ allocAndInitSong start ~~~~~~~\n");
printf("Adding a song\n");
printf("Current song count: %d. Current songs address: %p\n", songsCount, songs);
    songs = (Song*) realloc(songs, (songsCount + 1) * sizeof(Song));
    if (songs == NULL) exit(1);
printf("Songs reallocated to %p\n", songs);
    char tempTxt[maxNameSize];
    printf("Enter song's details\n");
    printf("Title:\n");
    scanf(" %[^\n]", &tempTxt);
printf("Title before allocation. address: %p str: %s\n", songs[songsCount].title, songs[songsCount].title);
    songs[songsCount].title = allocAndCopyString(tempTxt);
printf("Title after allocation. address: %p str: %s\n", songs[songsCount].title, songs[songsCount].title);
    printf("Artist:\n");
    scanf(" %[^\n]", &tempTxt);
printf("Artist before allocation. address: %p str: %s\n", songs[songsCount].artist, songs[songsCount].artist);
    songs[songsCount].artist = allocAndCopyString(tempTxt);
printf("Artist after allocation. address: %p str: %s\n", songs[songsCount].artist, songs[songsCount].artist);
    printf("Year of release:\n");
    scanf("%d", &songs[songsCount].year);
    printf("Lyrics:\n");
    scanf(" %[^\n]", &tempTxt);
printf("Lyrics before allocation. address: %p str: %s\n", songs[songsCount].lyrics, songs[songsCount].lyrics);
    songs[songsCount].lyrics = allocAndCopyString(tempTxt);
printf("Lyrics after allocation. address: %p str: %s\n", songs[songsCount].lyrics, songs[songsCount].lyrics);
    songs[songsCount].streams = 0;
printf("Song added.\n");
printf("~~~~~~~ allocAndInitSong end ~~~~~~~\n\n");
    return songs;
}


void addSong(Song** songs, int *songsCount) {
printf("~~~~~~~ addSong start ~~~~~~~\n");
printf("Songs count before add: %d\n", *songsCount);
printf("Songs address before call to allocAndInitSong: %p\n", *songs);
    *songs = allocAndInitSong(*songs, *songsCount);
printf("Songs address after call to allocAndInitSong: %p\n", *songs);
    *songsCount = *songsCount + 1;
printf("Songs count after add: %d\n", *songsCount);
printf("~~~~~~~ addSong end ~~~~~~~\n\n");
}


void showPlaylist(Song** songs, int songsCount) {
    int choice = -1;
    printPlaylist(*songs, songsCount);
    while (1) {
        printf("choose a song to play, or 0 to quit:\n");
        scanf("%d", &choice);
        if (choice > 0 && choice <= songsCount)
            playSong(&(*songs)[choice-1]);
        else break;
    }
}


void playPlaylist(Song** songs, int songsCount) {
    for (int i = 0; i < songsCount; i++) {
        playSong(&(*songs)[i]);
        printf("\n");
    }
}


void sortPlaylist(Song** songs, int songsCount) {
    int choice = menu(sortPlaylistMenuText);
    for (int i = 0; i < songsCount - 1; i++) {
        for (int j = 0; j < songsCount - 1 - i; j++) {
            switch (choice) {
                case 1:
                    if ((*songs)[j].year > (*songs)[j + 1].year)
                        switchSongs(&(*songs)[j], &(*songs)[j + 1]);
                break;
                case 2:
                    if ((*songs)[j].streams > (*songs)[j + 1].streams)
                        switchSongs(&(*songs)[j], &(*songs)[j + 1]);
                break;
                case 3:
                    if ((*songs)[j].streams < (*songs)[j + 1].streams)
                        switchSongs(&(*songs)[j], &(*songs)[j + 1]);
                break;
                default:
                    if (strcmp((*songs)[j].title, (*songs)[j + 1].title) > 0)
                        switchSongs(&(*songs)[j], &(*songs)[j + 1]);
                break;
            }
        }
    }
    printf("sorted\n");
}


void playlistMenu(Playlist* playlist) {
    printf("playlist %s:\n", playlist->name);
    int choice = menu(playlistMenuText);
    while (choice != 6) {
        switch (choice) {
            case 1:
                showPlaylist(&playlist->songs, playlist->songsCount);
            break;
            case 2:
                addSong(&playlist->songs, &playlist->songsCount);
            break;
            case 3:
                deleteSong(&playlist->songs, &playlist->songsCount);
            break;
            case 4:
                sortPlaylist(&playlist->songs, playlist->songsCount);
            break;
            case 5:
                playPlaylist(&playlist->songs, playlist->songsCount);
            break;
            default:
                printf("Invalid option\n");
            break;
        }
        choice = menu(playlistMenuText);
    }
}


void watchPlaylists(Playlist** playlists, int playlistsCount) {
    int choice = 0, playlistIndex = 0;
    printWatchPlaylistsMenu(*playlists, playlistsCount);
    scanf("%d", &choice);
    playlistIndex = choice - 1;
    while (playlistIndex != playlistsCount) {
        if (playlistIndex >= 0 && playlistIndex < playlistsCount) {
            playlistMenu(&(*playlists)[playlistIndex]);
        }
        else printf("Invalid option\n");
        printWatchPlaylistsMenu(*playlists, playlistsCount);
        scanf("%d", &choice);
        playlistIndex = choice - 1;
    }
}


Playlist* allocAndInitPlaylistInPlaylists(Playlist* playlists, int playlistsCount) {
printf("~~~~~~~ allocAndInitPlaylistInPlaylists start ~~~~~~~\n");
printf("Adding a playlist\n");
printf("Current playlist count: %d. Current playlists address: %p\n", playlistsCount, playlists);
    playlists = (Playlist*) realloc(playlists, (playlistsCount + 1) * sizeof(Playlist));
    if (playlists == NULL) exit(1);
printf("Playlists reallocated to %p\n", playlists);
    playlists[playlistsCount].name = NULL;
    playlists[playlistsCount].songs = NULL;
    playlists[playlistsCount].songsCount = 0;
printf("~~~~~~~ allocAndInitPlaylistInPlaylists end ~~~~~~~\n\n");
    return playlists;
}


void addPlaylist(Playlist** playlists, int *playlistsCount) {
printf("~~~~~~~ addPlaylist start ~~~~~~~\n");
    char playlistName[maxNameSize];
    printf("Enter playlist's name:\n");
    scanf("%s", playlistName);
printf("Playlists address before call to allocAndInitPlaylistInPlaylists: %p\n", *playlists);
    *playlists = allocAndInitPlaylistInPlaylists(*playlists, *playlistsCount);
printf("Playlists address after call to allocAndInitPlaylistInPlaylists: %p\n", *playlists);
printf("Name before allocation. address: %p str: %s\n", (*playlists)[*playlistsCount].name, (*playlists)[*playlistsCount].name);
    (*playlists)[*playlistsCount].name = allocAndCopyString(playlistName);
printf("Name after allocation. address: %p str: %s\n", (*playlists)[*playlistsCount].name, (*playlists)[*playlistsCount].name);
    *playlistsCount = *playlistsCount + 1;
printf("Playlists count after add: %d\n", *playlistsCount);
printf("~~~~~~~ addPlaylist end ~~~~~~~\n\n");
}


void switchPlaylists(Playlist* playlist1, Playlist* playlist2) {
    char* tempString;
    int tempInt;
    Song* tempSong;

    tempString = playlist1->name;
    playlist1->name = playlist2->name;
    playlist2->name = tempString;

    tempSong = playlist1->songs;
    playlist1->songs = playlist2->songs;
    playlist2->songs = tempSong;

    tempInt = playlist1->songsCount;
    playlist1->songsCount = playlist2->songsCount;
    playlist2->songsCount = tempInt;
}


void freePlaylist(Playlist **playlists, int *playlistsCount, int playlistIndexToFree) {
printf("~~~~~~~ freePlaylist start ~~~~~~~\n");
printf("Freeing playlist %d\n", playlistIndexToFree);
printf("Freeing all songs...\n");
    // delete all songs
    while ((*playlists)[playlistIndexToFree].songsCount > 0) {
        freeSong(&(*playlists)[playlistIndexToFree].songs, &(*playlists)[playlistIndexToFree].songsCount, 0);
    }
printf("All songs freed.\n");
printf("Freeing name %s from %p\n", (*playlists)[playlistIndexToFree].name, (*playlists)[playlistIndexToFree].name);
    free((*playlists)[playlistIndexToFree].name);
    // sending the going-to-be-deleted playlist to the end of the list
printf("Pushing freed playlist to end of array, verifying...\n");
printf("Current name of last playlist is %s\n", (*playlists)[*playlistsCount - 1].name);
    for (int i = playlistIndexToFree; i < *playlistsCount - 1; i++) {
printf("Pushing...\n");
        switchPlaylists(&(*playlists)[i], &(*playlists)[i + 1]);
    }
printf("Name of last playlist after push is %s\n", (*playlists)[*playlistsCount - 1].name);
printf("Removing playlist from playlists. Reminder, playlists count before delete is: %d, playlists address is %p\n", *playlistsCount, *playlists);
    if (*playlistsCount == 1) {
printf("Freeing...\n");
        free(*playlists);
printf("Freed.\n");
    }
    else {
        *playlists = realloc(*playlists, (*playlistsCount - 1) * sizeof(Playlist));
printf("Playlists reallocated to %p\n", *playlists);
    }
    *playlistsCount = *playlistsCount - 1;
printf("Playlists count now is: %d\n", *playlistsCount);
printf("Playlist freed.\n");
printf("~~~~~~~ freePlaylist end ~~~~~~~\n\n");
}


void deletePlaylist(Playlist** playlists, int *playlistsCount) {
    int choice = -1, playlistIndexToFree = -1;
    printWatchPlaylistsMenu(*playlists, *playlistsCount);
    scanf("%d", &choice);
    playlistIndexToFree = choice - 1;
    if (playlistIndexToFree >= 0 && playlistIndexToFree < *playlistsCount) {
        freePlaylist(playlists, playlistsCount, playlistIndexToFree);
        printf("Playlist deleted.\n");
    }
}


int main() {
    int choice = 0, playlistsCount = 0;
    Playlist* playlists = NULL;
    choice = menu(mainMenuText);
    while (choice != 4) {
        switch (choice) {
            case 1:
                watchPlaylists(&playlists, playlistsCount);
                break;
            case 2:
                addPlaylist(&playlists, &playlistsCount);
                break;
            case 3:
                deletePlaylist(&playlists, &playlistsCount);
                break;
            default:
                printf("Invalid option\n");
                break;
        }
        choice = menu(mainMenuText);
    }
    for (int i = 0; i < playlistsCount; i++)
        freePlaylist(&playlists, &playlistsCount, i);
    printf("Goodbye!\n");
}
