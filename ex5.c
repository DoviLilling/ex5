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

#define mainMenuText "Please Choose:\n\t1. Watch playlists\n\t2. Add playlist\n\t3. Remove playlist\n\t4. exit\n"
#define playlistMenuText "Please Choose:\n\t1. Show playlist\n\t2. Add song\n\t3. Delete Song\n\t4. Sort\n\t5. Play\n\t6. exit\n"
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
        // printf("\t%d. %s\n", i + 1, playlists[i].name);
printf("\t%d. %s (address: %p)\n", i + 1, playlists[i].name, &playlists[i]);
    }
    printf("\t%d. Back to main menu\n", playlistsCount + 1);
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
    printf("Now playing %s:\n♪ %s ♪\n", song->title, song->lyrics);
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
printf("* after realloc *\n");
printf("songs points to %p\n", songs);
printf("size of songs: %lu\n", sizeof(*songs));
    if (songs == NULL) exit(1);
    char tempTxt[maxNameSize];
    printf("Enter songs's details:\n");
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
printf("*** addSong ***\n");
printf("songs address: %p\n", &songs);
printf("songs points to %p, which points to %p\n", songs, *songs);
printf("songsCount: %d\n", *songsCount);
printf("songs[*songsCount] address: %p and it points to: %p\n", &songs[*songsCount], songs[*songsCount]);
    *songs = allocAndInitSong(*songs, *songsCount);
printf("* after allocAndInitSong *\n");
printf("songs address: %p\n", &songs);
printf("songs points to %p, which points to %p\n", songs, *songs);
printf("(*songs)[*songsCount] address is %p\n", &(*songs)[*songsCount]);
printf("addresses of songs[*songsCount]->title: %p, and the title is: %s\n", &(*songs)[*songsCount].title, (*songs)[*songsCount].title);
printf("addresses of songs[*songsCount]->artist: %p, and the artist is: %s\n", &(*songs)[*songsCount].artist, (*songs)[*songsCount].artist);
printf("addresses of songs[*songsCount]->year: %p, and the year is: %d\n", &(*songs)[*songsCount].year, (*songs)[*songsCount].year);
printf("addresses of songs[*songsCount]->lyrics: %p, and the lyrics are: %s\n", &(*songs)[*songsCount].lyrics, (*songs)[*songsCount].lyrics);
printf("addresses of songs[*songsCount]->title: %p, and the title is: %d\n", &(*songs)[*songsCount].streams, (*songs)[*songsCount].streams);
    *songsCount = *songsCount + 1;
printf("songsCount: %d\n", *songsCount);
    printf("*** addSong done ***\n");
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
    for (int i = 0; i < songsCount; i++)
        playSong(&(*songs)[i]);
}


int compareIntsAscending(int* a, int* b) {return *a - *b;}
int compareIntsDescending(int* a, int* b) {return *b - *a;}
int compareStrings(char* a, char* b) {return strcmp(a, b);}


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
printf("*** playlistMenu ***\n");
printf("playlist address: %p\n", &playlist);
printf("playlist points to %p\n", playlist);
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
printf("*** watchPlaylists ***\n");
printf("playlists address: %p\n", &playlists);
printf("playlists points to %p, which points to %p\n", playlists, *playlists);
    int choice = 0, playlistIndex = 0;
    printWatchPlaylistsMenu(*playlists, playlistsCount);
    scanf("%d", &choice);
    playlistIndex = choice - 1;
    while (playlistIndex != playlistsCount) {
        if (playlistIndex >= 0 && playlistIndex < playlistsCount) {
printf("playlistIndex: %d\n", playlistIndex);
printf("size of a playlist struct: %lu\n", sizeof(Playlist));
printf("playlists[choice] address: %p\n", &playlists[playlistIndex]);
printf("playlists[choice] points to %p, which points to %p\n", playlists[playlistIndex], &(*playlists)[playlistIndex]);
            playlistMenu(&(*playlists)[playlistIndex]);
        }
        else printf("Invalid option\n");
        printWatchPlaylistsMenu(*playlists, playlistsCount);
        scanf("%d", &choice);
        playlistIndex = choice - 1;
    }
printf("*** watchPlaylists done ***\n");
}


Playlist* allocAndInitPlaylistInPlaylists(Playlist playlists[], int playlistsCount) {
    playlists = (Playlist*) realloc(playlists, (playlistsCount + 1) * sizeof(Playlist));
printf("* after realloc *\n");
printf("playlists points to %p\n", playlists);
printf("size of playlists: %lu\n", sizeof(*playlists));
    if (playlists == NULL) exit(1);
    playlists[playlistsCount].name = NULL;
    playlists[playlistsCount].songs = NULL;
    playlists[playlistsCount].songsCount = 0;
    return playlists;
}


void addPlaylist(Playlist* playlists[], int *playlistsCount) {
printf("*** addPlaylist ***\n");
printf("playlists address: %p\n", &playlists);
printf("playlists points to %p, which points to %p\n", playlists, *playlists);
printf("playlistsCount: %d\n", *playlistsCount);
printf("playlists[*playlistsCount] address: %p and it points to: %p\n", &playlists[*playlistsCount], playlists[*playlistsCount]);
    char playlistName[maxNameSize];
    printf("Enter playlist's name:\n");
    scanf("%s", playlistName);
    *playlists = allocAndInitPlaylistInPlaylists(*playlists, *playlistsCount);
printf("* after allocAndInitPlaylistInPlaylists *\n");
printf("playlists address: %p\n", &playlists);
printf("playlists points to %p, which points to %p\n", playlists, *playlists);
printf("playlists[*playlistsCount] address is %p, and it points to %p\n", &(*playlists)[*playlistsCount], playlists[*playlistsCount]);
printf("addresses of playlists[*playlistsCount]->name: %p, playlistName: %p\n", &playlists[*playlistsCount]->name, playlistName);
    allocAndCopyString(playlistName, &(*playlists)[*playlistsCount].name);
// printf("copied\n");
    *playlistsCount = *playlistsCount + 1;
printf("*** addPlaylist done ***\n");
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
printf("size of a pointer: %lu\n", sizeof(Playlist*));
printf("*** MAIN ***\n");
printf("playlists address: %p\n", &playlists);
printf("playlists points to: %p\n", playlists);
printf("size of playlists: %lu\n", sizeof(*playlists));
    choice = menu(mainMenuText);
    while (choice != 4) {
printf("*** MAIN ***\n");
printf("playlists address: %p\n", &playlists);
printf("playlists points to: %p\n", playlists);
printf("size of playlists: %lu\n", sizeof(*playlists));
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
printf("playlistsCount: %d\n", playlistsCount);
        choice = menu(mainMenuText);
    }
    printf("Goodbye!\n");
}
