#include "DataBase.h"
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QObject>
#include <QSqlQuery>
#include "playlist.h"

/**
 * @class DataBase
 * Diese Klasse verwaltet die SQLite-Datenbank des Musikplayers. Sie stellt Funktionen
 * bereit, um Datenbanktabellen zu erstellen, Daten einzufügen, Daten abzufragen und
 * die Konsistenz der Daten zu überprüfen.
 */

DataBase::DataBase()
{
    // Initialisierung der Datenbankverbindung
    if (QSqlDatabase::contains("MusicPlayerConnection")) {
        db = QSqlDatabase::database("MusicPlayerConnection");
    } else {
        db = QSqlDatabase::addDatabase("QSQLITE", "MusicPlayerConnection");
        db.setDatabaseName("MusicPlayerSQLDB.db");

        if (!db.open()) {
            return;
        }
    }

    // Tabellen in der Datenbank erstellen
    createTableMediathek();
    createTablePathlist();
    createTablePlaylists();
    createTablePlaylistTracks();
}

DataBase::~DataBase()
{
    db.close(); // Datenbankverbindung schließen
}

/**
 * Öffnet die Datenbankverbindung, falls sie nicht bereits geöffnet ist.
 * @return true, wenn die Verbindung erfolgreich geöffnet wurde, sonst false.
 */
bool DataBase::open()
{
    if (!db.isOpen()) {
        if (!db.open()) {
            return false;
        }
    }
    return true;
}

/**
 * Schließt die Datenbankverbindung.
 */
void DataBase::close()
{
    db.close();
}

/**
 * Erstellt die Tabelle "Mediathek" in der Datenbank, falls sie nicht existiert.
 * Diese Tabelle speichert Informationen über Musikdateien.
 * @return true, wenn die Tabelle erfolgreich erstellt wurde, sonst false.
 */
bool DataBase::createTableMediathek()
{
    if (!open()) {
        return false;
    }

    if (!QSqlDatabase::isDriverAvailable("QSQLITE")) {
        return false;
    }

    QSqlQuery query(db);
    bool success = query.prepare("CREATE TABLE IF NOT EXISTS Mediathek ("
                                 "TrackID INTEGER PRIMARY KEY AUTOINCREMENT, "
                                 "FilePath TEXT, "
                                 "Interpret TEXT, "
                                 "Titel TEXT, "
                                 "Album TEXT, "
                                 "Spielzeit INT, "
                                 "SampleRate INT)");
    query.exec();
    return success;
}

/**
 * Erstellt die Tabelle "Pathlist", falls sie nicht existiert.
 * Diese Tabelle speichert Verzeichnispfade, die durchsucht werden sollen.
 * @return true, wenn die Tabelle erfolgreich erstellt wurde, sonst false.
 */
bool DataBase::createTablePathlist()
{
    if (!open()) {
        return false;
    }

    QSqlQuery query(db);
    bool success = query.exec("CREATE TABLE IF NOT EXISTS Pathlist ("
                              "ID INTEGER PRIMARY KEY AUTOINCREMENT,"
                              "MusikPfad TEXT)");
    return success;
}

/**
 * Erstellt die Tabelle "Playlists", falls sie nicht existiert.
 * Diese Tabelle speichert Informationen über erstellte Playlists.
 * @return true, wenn die Tabelle erfolgreich erstellt wurde, sonst false.
 */
bool DataBase::createTablePlaylists()
{
    if (!open()) {
        return false;
    }

    QSqlQuery query(db);
    bool success = query.exec("CREATE TABLE IF NOT EXISTS Playlists ("
                              "PlaylistID INTEGER PRIMARY KEY AUTOINCREMENT, "
                              "Name TEXT)");
    return success;
}

/**
 * Erstellt die Tabelle "PlaylistTracks", falls sie nicht existiert.
 * Diese Tabelle verknüpft Playlists mit den darin enthaltenen Tracks.
 * @return true, wenn die Tabelle erfolgreich erstellt wurde, sonst false.
 */
bool DataBase::createTablePlaylistTracks()
{
    if (!open()) {
        return false;
    }

    QSqlQuery query(db);
    bool success = query.exec("CREATE TABLE IF NOT EXISTS PlaylistTracks ("
                              "PlaylistID INTEGER, "
                              "TrackID INTEGER, "
                              "FOREIGN KEY (PlaylistID) REFERENCES Playlists(PlaylistID), "
                              "FOREIGN KEY (TrackID) REFERENCES Mediathek(TrackID))");
    return success;
}

/**
 * Fügt einen neuen Track in die Mediathek ein.
 * @param filePath Pfad der Datei.
 * @param interpret Name des Interpreten.
 * @param album Name des Albums.
 * @param titel Titel des Tracks.
 * @param spielzeit Dauer des Tracks in Sekunden.
 * @param sampleRate Abtastrate des Tracks.
 * @return true, wenn der Track erfolgreich eingefügt wurde, sonst false.
 */
bool DataBase::insertData(const QString &filePath,
                          const QString &interpret,
                          const QString &album,
                          const QString &titel,
                          int spielzeit,
                          int sampleRate)
{
    if (!open()) {
        return false;
    }

    QSqlQuery query(db);
    query.prepare("SELECT TrackID FROM Mediathek WHERE FilePath = :filePath");
    query.bindValue(":filePath", filePath);

    if (query.exec() && query.next()) {
        return false; // Track existiert bereits
    }

    query.prepare("INSERT INTO Mediathek (FilePath, Interpret, Album, Titel, Spielzeit, SampleRate) "
                  "VALUES (:filePath, :interpret, :album, :titel, :spielzeit, :sampleRate)");
    query.bindValue(":filePath", filePath);
    query.bindValue(":interpret", interpret);
    query.bindValue(":album", album);
    query.bindValue(":titel", titel);
    query.bindValue(":spielzeit", spielzeit);
    query.bindValue(":sampleRate", sampleRate);

    if (!query.exec()) {
        return false;
    }

    int trackID = query.lastInsertId().toInt();
    return insertPlaylistTrack(1, trackID);
}

/**
 * Fügt einen neuen Verzeichnispfad in die Pathlist ein.
 * @param path Der zu speichernde Verzeichnispfad.
 * @return true, wenn der Pfad erfolgreich eingefügt wurde, sonst false.
 */
bool DataBase::insertPath(const QString &path)
{
    if (!open()) {
        return false;
    }

    QSqlQuery query(db);
    query.prepare("INSERT INTO Pathlist (MusikPfad) VALUES (:MusikPfad)");
    query.bindValue(":MusikPfad", path);

    if (!query.exec()) {
        return false;
    }

    return true;
}

/**
 * Fügt eine neue Playlist in die Datenbank ein.
 * @param playlistName Name der neuen Playlist.
 * @return true, wenn die Playlist erfolgreich eingefügt wurde, sonst false.
 */
bool DataBase::insertPlaylist(const QString &playlistName)
{
    if (!open()) {
        return false;
    }

    QSqlQuery query(db);

    // Überprüfen, ob die Playlist bereits existiert
    query.prepare("SELECT COUNT(*) FROM Playlists WHERE Name = :name");
    query.bindValue(":name", playlistName);
    if (query.exec() && query.next()) {
        if (query.value(0).toInt() > 0) {
            return false;
        }
    }

    query.prepare("INSERT INTO Playlists (Name) VALUES (:name)");
    query.bindValue(":name", playlistName);

    if (!query.exec()) {
        return false;
    }

    return true;
}

/**
 * Fügt einen Track zu einer Playlist hinzu.
 * @param playlistID ID der Playlist.
 * @param trackID ID des Tracks.
 * @return true, wenn der Track erfolgreich hinzugefügt wurde, sonst false.
 */
bool DataBase::insertPlaylistTrack(int playlistID, int trackID)
{
    if (!open()) {
        return false;
    }

    QSqlQuery query(db);

    // Überprüfen, ob der Track bereits in der Playlist enthalten ist
    query.prepare("SELECT COUNT(*) FROM PlaylistTracks WHERE PlaylistID = :playlistID AND TrackID = :trackID");
    query.bindValue(":playlistID", playlistID);
    query.bindValue(":trackID", trackID);

    if (query.exec() && query.next()) {
        if (query.value(0).toInt() > 0) {
            QMessageBox::warning(nullptr, "Fehler", "Track ist bereits in der Playlist enthalten.");
            return false;
        }
    }

    query.prepare("INSERT INTO PlaylistTracks (PlaylistID, TrackID) VALUES (:playlistID, :trackID)");
    query.bindValue(":playlistID", playlistID);
    query.bindValue(":trackID", trackID);

    if (!query.exec()) {
        return false;
    }

    return true;
}

/**
 * Entfernt einen Track aus einer Playlist in der Datenbank.
 * @param playlistID ID der Playlist.
 * @param trackID ID des Tracks.
 * @return true, wenn der Track erfolgreich entfernt wurde, sonst false.
 */
bool DataBase::removeTrackFromPlaylist(int playlistID, int trackID)
{
    if (!open()) {
        return false; // Datenbank konnte nicht geöffnet werden.
    }

    QSqlQuery query(db);
    query.prepare(
        "DELETE FROM PlaylistTracks WHERE PlaylistID = :playlistID AND TrackID = :trackID");
    query.bindValue(":playlistID", playlistID);
    query.bindValue(":trackID", trackID);

    if (!query.exec()) {
        return false; // Fehler beim Ausführen der Löschabfrage.
    }

    return true;
}

/**
 * Prüft, ob eine bestimmte Tabelle in der Datenbank existiert.
 * @param tableName Name der Tabelle.
 * @return true, wenn die Tabelle existiert, sonst false.
 */
bool DataBase::tableExists(const QString &tableName)
{
    if (!open()) {
        return false; // Datenbank konnte nicht geöffnet werden.
    }
    QSqlQuery query(db);
    query.prepare("SELECT name FROM sqlite_master WHERE type='table' AND name=:tableName");
    query.bindValue(":tableName", tableName);
    if (!query.exec()) {
        return false; // Fehler beim Ausführen der Abfrage.
    }
    return query.next(); // Prüft, ob das Ergebnis eine Zeile enthält.
}

/**
 * Gibt eine Referenz auf das QSqlDatabase-Objekt zurück.
 * @return Referenz auf die Datenbank.
 */
QSqlDatabase &DataBase::getDatabase()
{
    return db; // Rückgabe des internen QSqlDatabase-Objekts.
}

/**
 * Lädt eine Playlist aus der Datenbank anhand ihres Namens.
 * @param playlistName Name der Playlist.
 * @return Die geladene Playlist.
 */
Playlist DataBase::getPlaylist(const QString &playlistName)
{
    Playlist playlist;
    playlist.setName(playlistName);

    if (!db.isOpen()) {
        if (!open()) {
            return playlist; // Rückgabe einer leeren Playlist, wenn die Datenbank nicht geöffnet werden kann.
        }
    }

    QSqlQuery query(db);
    query.prepare("SELECT PlaylistID FROM Playlists WHERE Name = :name");
    query.bindValue(":name", playlistName);
    if (!query.exec() || !query.next()) {
        return playlist; // Rückgabe einer leeren Playlist, wenn keine passende gefunden wurde.
    }
    int playlistID = query.value(0).toInt();

    playlist.setPlaylistID(playlistID);
    playlist.setTracks(std::vector<Track>()); // Initialisiert die Trackliste der Playlist.

    query.prepare("SELECT Mediathek.TrackID, Mediathek.FilePath, Mediathek.Interpret, "
                  "Mediathek.Titel, Mediathek.Album, Mediathek.Spielzeit, Mediathek.SampleRate "
                  "FROM PlaylistTracks "
                  "JOIN Mediathek ON PlaylistTracks.TrackID = Mediathek.TrackID "
                  "WHERE PlaylistTracks.PlaylistID = :playlistID");
    query.bindValue(":playlistID", playlistID);
    if (query.exec()) {
        while (query.next()) {
            Track track;
            track.setTrackID(query.value("TrackID").toInt());
            track.setFilePath(query.value("FilePath").toString());
            track.setArtist(query.value("Interpret").toString());
            track.setTitle(query.value("Titel").toString());
            track.setAlbum(query.value("Album").toString());
            track.setDuration(query.value("Spielzeit").toInt());
            track.setSampleRate(query.value("SampleRate").toInt());
            playlist.addTrack(track); // Fügt den Track zur Playlist hinzu.
        }
    }

    return playlist;
}

/**
 * Lädt alle Playlists aus der Datenbank.
 * @return Eine Liste aller Playlists.
 */
std::vector<Playlist> DataBase::getAllPlaylists()
{
    std::vector<Playlist> playlists;

    if (!open()) {
        return playlists; // Rückgabe einer leeren Liste, wenn die Datenbank nicht geöffnet werden kann.
    }

    QSqlQuery query(db);
    query.prepare("SELECT PlaylistID, Name FROM Playlists");
    if (!query.exec()) {
        return playlists; // Rückgabe einer leeren Liste, wenn die Abfrage fehlschlägt.
    }

    while (query.next()) {
        int playlistID = query.value("PlaylistID").toInt();
        QString playlistName = query.value("Name").toString();
        Playlist playlist;
        playlist.setPlaylistID(playlistID);
        playlist.setName(playlistName);
        playlists.push_back(playlist); // Fügt die Playlist zur Liste hinzu.
    }

    return playlists;
}

/**
 * Entfernt eine Playlist aus der Datenbank, außer sie heißt "Alle Songs".
 * @param playlistName Name der zu entfernenden Playlist.
 * @return true, wenn die Playlist erfolgreich entfernt wurde, sonst false.
 */
bool DataBase::removePlaylist(const QString &playlistName)
{
    if (playlistName == "Alle Songs") {
        return false; // Die Playlist "Alle Songs" kann nicht gelöscht werden.
    }

    if (!open()) {
        return false; // Datenbank konnte nicht geöffnet werden.
    }

    QSqlQuery query(db);
    query.prepare("DELETE FROM Playlists WHERE Name = :name");
    query.bindValue(":name", playlistName);

    if (!query.exec()) {
        return false; // Fehler beim Löschen der Playlist.
    }

    query.prepare("DELETE FROM PlaylistTracks WHERE PlaylistID = (SELECT PlaylistID FROM Playlists "
                  "WHERE Name = :name)");
    query.bindValue(":name", playlistName);

    if (!query.exec()) {
        return false; // Fehler beim Löschen der zugehörigen Tracks.
    }

    return true;
}

/**
 * Lädt einen Track aus der Mediathek anhand des Dateipfades.
 * @param filePath Dateipfad des Tracks.
 * @return Der geladene Track.
 */
Track DataBase::getTrack(const QString &filePath)
{
    QSqlQuery query(db);
    query.prepare("SELECT TrackID, FilePath, Interpret, Titel, Album, Spielzeit, SampleRate FROM "
                  "Mediathek WHERE FilePath = :filePath");
    query.bindValue(":filePath", filePath);
    if (query.exec() && query.next()) {
        Track track;
        track.setTrackID(query.value("TrackID").toInt());
        track.setFilePath(query.value("FilePath").toString());
        track.setArtist(query.value("Interpret").toString());
        track.setTitle(query.value("Titel").toString());
        track.setAlbum(query.value("Album").toString());
        track.setDuration(query.value("Spielzeit").toInt());
        track.setSampleRate(query.value("SampleRate").toInt());
        return track; // Rückgabe des gefundenen Tracks.
    }
    return Track(); // Rückgabe eines leeren Tracks, wenn nichts gefunden wurde.
}

/**
 * Überprüft die Existenz von Mediendateien und aktualisiert die Datenbank entsprechend.
 */
void DataBase::checkFiles()
{
    if (!open()) {
        return; // Datenbank konnte nicht geöffnet werden.
    }
    QSqlQuery query(getDatabase());
    query.prepare("SELECT TrackID, FilePath FROM Mediathek");
    if (query.exec()) {
        while (query.next()) {
            int trackID = query.value("TrackID").toInt();
            QString filePath = query.value("FilePath").toString();
            if (!QFile::exists(filePath)) {
                QSqlQuery deleteQuery(getDatabase());
                deleteQuery.prepare("DELETE FROM Mediathek WHERE TrackID = :trackID");
                deleteQuery.bindValue(":trackID", trackID);
                deleteQuery.exec(); // Löscht den ungültigen Eintrag aus der Mediathek.

                deleteQuery.prepare("DELETE FROM PlaylistTracks WHERE TrackID = :trackID");
                deleteQuery.bindValue(":trackID", trackID);
                deleteQuery.exec(); // Löscht den ungültigen Eintrag aus PlaylistTracks.
            }
        }
    }

    query.prepare("SELECT MusikPfad FROM Pathlist");
    if (query.exec()) {
        while (query.next()) {
            QString directoryPath = query.value("MusikPfad").toString();
            QDir directory(directoryPath);
            if (directory.exists()) {
                processDirectory(directory); // Verarbeitet das gültige Verzeichnis.
            } else {
                QSqlQuery deleteQuery(getDatabase());
                deleteQuery.prepare("DELETE FROM Pathlist WHERE MusikPfad = :directoryPath");
                deleteQuery.bindValue(":directoryPath", directoryPath);
                deleteQuery.exec(); // Entfernt ungültige Verzeichniseinträge.
            }
        }
    }
}

/**
 * Fügt alle Mediendateien aus einer Liste in die Datenbank ein.
 * @param basePath Basisverzeichnis der Dateien.
 * @param fileList Liste der Dateinamen.
 */
void DataBase::processFiles(const QString &basePath, const QStringList &fileList)
{
    foreach (const QString &fileName, fileList) {
        Track track(QDir(basePath).absoluteFilePath(fileName));
        insertData(track.getFilePath(),
                   track.getArtist(),
                   track.getAlbum(),
                   track.getTitle(),
                   track.getDuration(),
                   track.getSampleRate()); // Fügt die Datei in die Datenbank ein.
    }
}

/**
 * Verarbeitet alle Mediendateien und Unterverzeichnisse eines Verzeichnisses.
 * @param directory Verzeichnis, das verarbeitet werden soll.
 */
void DataBase::processDirectory(const QDir &directory)
{
    QStringList audioFiles = directory.entryList(QStringList()
                                                 << "*.mp3" << "*.wav" << "*.flac" << "*.aac",
                                                    QDir::Files);
    processFiles(directory.absolutePath(), audioFiles); // Verarbeitet die Audio-Dateien.

    QStringList subDirs = directory.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    foreach (const QString &subDir, subDirs) {
        QDir subDirectory(directory.absoluteFilePath(subDir));
        processDirectory(subDirectory); // Rekursives Verarbeiten der Unterverzeichnisse.
    }
}
