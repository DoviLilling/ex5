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


void freeSong(Song* songs[], int* songsCount, int songIndexToFree) {
    // sending the deleted song to the end of the list
    for (int i = songIndexToFree - 1; i < *songsCount - 1; i++)
        switchSongs(&(*songs)[i], &(*songs)[i + 1]);
    free((*songs)[*songsCount-1].title);
    free((*songs)[*songsCount-1].artist);
    free((*songs)[*songsCount-1].lyrics);
    *songs = realloc(*songs, (*songsCount - 1) * sizeof(Song));
    *songsCount = *songsCount - 1;
}

void deleteSong(Song* songs[], int *songsCount) {
    int choice = -1;
    printPlaylist(*songs, *songsCount);
    printf("choose a song to delete, or 0 to quit:\n");
    scanf("%d", &choice);
    if (choice > 0 && choice <= *songsCount)
        freeSong(songs, songsCount, choice);
    printf("Song deleted successfully.\n");
}


void playSong(Song* song) {
    printf("Now playing %s:\n%s\n", song->title, song->lyrics);
    song->streams++;
}


void allocAndCopyString(char* source, char** target) {
    *target = NULL;
    *target = (char *) realloc(*target, (strlen(source) + 1 ) * sizeof(char));
    if (*target == NULL) exit(1);
    strcpy(*target, source);
}


Song* allocAndInitSong(Song songs[], int songsCount) {
    songs = (Song*) realloc(songs, (songsCount + 1) * sizeof(Song));
    if (songs == NULL) exit(1);
    char tempTxt[maxNameSize];
    printf("Enter song's details\n");
    printf("Title:\n");
    scanf(" %[^\n]", &tempTxt);
    allocAndCopyString(tempTxt, &songs[songsCount].title);
    printf("Artist:\n");
    scanf(" %[^\n]", &tempTxt);
    allocAndCopyString(tempTxt, &songs[songsCount].artist);
    printf("Year of release:\n");
    scanf("%d", &songs[songsCount].year);
    printf("Lyrics:\n");
    scanf(" %[^\n]", &tempTxt);
    allocAndCopyString(tempTxt, &songs[songsCount].lyrics);
    songs[songsCount].streams = 0;
    return songs;
}


void addSong(Song* songs[], int *songsCount) {
    *songs = allocAndInitSong(*songs, *songsCount);
    *songsCount = *songsCount + 1;
}


void showPlaylist(Song* songs[], int songsCount) {
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


void playPlaylist(Song* songs[], int songsCount) {
    for (int i = 0; i < songsCount; i++) {
        playSong(&(*songs)[i]);
        printf("\n");
    }
}


void sortPlaylist(Song* songs[], int songsCount) {
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


Playlist* allocAndInitPlaylistInPlaylists(Playlist playlists[], int playlistsCount) {
    playlists = (Playlist*) realloc(playlists, (playlistsCount + 1) * sizeof(Playlist));
    if (playlists == NULL) exit(1);
    playlists[playlistsCount].name = NULL;
    playlists[playlistsCount].songs = NULL;
    playlists[playlistsCount].songsCount = 0;
    return playlists;
}


void addPlaylist(Playlist* playlists[], int *playlistsCount) {
    char playlistName[maxNameSize];
    printf("Enter playlist's name:\n");
    scanf("%s", playlistName);
    *playlists = allocAndInitPlaylistInPlaylists(*playlists, *playlistsCount);
    allocAndCopyString(playlistName, &(*playlists)[*playlistsCount].name);
    *playlistsCount = *playlistsCount + 1;
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


void deletePlaylist(Playlist* playlists[], int *playlistsCount) {
    int choice = -1, playlistIndexToFree = -1;
    printWatchPlaylistsMenu(*playlists, *playlistsCount);
    scanf("%d", &choice);
    playlistIndexToFree = choice - 1;
    if (playlistIndexToFree >= 0 && playlistIndexToFree < *playlistsCount) {
        // delete all songs
        for (int i = 0; i < (*playlists)[playlistIndexToFree].songsCount; i++) {
            freeSong(&(*playlists)[playlistIndexToFree].songs, &(*playlists)[playlistIndexToFree].songsCount, 1);
        }
        free((*playlists)[playlistIndexToFree].songs);
        free((*playlists)[playlistIndexToFree].name);
        // sending the going-to-be-deleted playlist to the end of the list
        for (int i = playlistIndexToFree - 1; i < *playlistsCount - 1; i++)
            switchPlaylists(&(*playlists)[i], &(*playlists)[i + 1]);
        *playlists = realloc(*playlists, (*playlistsCount - 1) * sizeof(Playlist));
        *playlistsCount = *playlistsCount - 1;
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
    printf("Goodbye!\n");
}
