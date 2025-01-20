#include "selecttrackdialog.h"
#include <QHeaderView>
#include "track.h"

/**
 * Konstruktor des SelectTrackDialog.
 * Initialisiert den Dialog und füllt die Tabelle mit den übergebenen Tracks.
 * @param tracks Die Liste der verfügbaren Tracks.
 * @param parent Das übergeordnete Widget, falls vorhanden.
 */
SelectTrackDialog::SelectTrackDialog(const std::vector<Track> &tracks, QWidget *parent)
    : QDialog(parent)
    , selectedTrack()
    , allTracks(tracks)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Suchleiste hinzufügen
    searchBar = new QLineEdit(this);
    searchBar->setPlaceholderText(tr("Search..."));
    layout->addWidget(searchBar);

    // Tabelle zur Anzeige der Tracks konfigurieren
    trackTableView = new QTableView(this);
    model = new QStandardItemModel(this);
    model->setHorizontalHeaderLabels({"#", "Title", "Artist", "Duration"});
    trackTableView->setModel(model);
    trackTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    trackTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    trackTableView->horizontalHeader()->setStretchLastSection(true);
    trackTableView->verticalHeader()->setVisible(false);
    trackTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    layout->addWidget(trackTableView);

    // Button zur Bestätigung der Auswahl
    selectButton = new QPushButton(tr("Select"), this);
    layout->addWidget(selectButton);

    // Verbindungen für Benutzerinteraktionen
    connect(selectButton, &QPushButton::clicked, this, &SelectTrackDialog::onTrackSelected);
    connect(searchBar, &QLineEdit::textChanged, this, &SelectTrackDialog::onSearchTextChanged);

    // Tabelle mit Tracks füllen
    int row = 0;
    for (const auto &track : tracks) {
        QStandardItem *indexItem = new QStandardItem(QString::number(row + 1));
        QStandardItem *titleItem = new QStandardItem(track.getTitle());
        QStandardItem *artistItem = new QStandardItem(track.getArtist());
        QStandardItem *durationItem = new QStandardItem(track.getSongDurationAsString());
        indexItem->setTextAlignment(Qt::AlignCenter);
        durationItem->setTextAlignment(Qt::AlignCenter);
        model->setItem(row, 0, indexItem);
        model->setItem(row, 1, titleItem);
        model->setItem(row, 2, artistItem);
        model->setItem(row, 3, durationItem);
        indexItem->setData(QVariant::fromValue(track), Qt::UserRole);
        ++row;
    }

    // Dialogbreite festlegen
    setFixedWidth(800);
}

/**
 * Gibt den vom Benutzer ausgewählten Track zurück.
 * @return Der ausgewählte Track.
 */
Track SelectTrackDialog::getSelectedTrack() const
{
    return selectedTrack;
}

/**
 * Slot, der ausgeführt wird, wenn der Benutzer einen Track auswählt.
 * Setzt den ausgewählten Track und schließt den Dialog.
 */
void SelectTrackDialog::onTrackSelected()
{
    QModelIndexList selectedRows = trackTableView->selectionModel()->selectedRows();
    if (!selectedRows.isEmpty()) {
        QModelIndex index = selectedRows.first();
        selectedTrack = model->item(index.row(), 0)->data(Qt::UserRole).value<Track>();
        accept();
    }
}

/**
 * Slot, der ausgeführt wird, wenn der Benutzer Text in die Suchleiste eingibt.
 * Filtert die angezeigten Tracks basierend auf dem Suchtext.
 * @param text Der eingegebene Suchtext.
 */
void SelectTrackDialog::onSearchTextChanged(const QString &text)
{
    for (int i = 0; i < model->rowCount(); ++i) {
        bool match = false;
        for (int j = 0; j < model->columnCount(); ++j) {
            QStandardItem *item = model->item(i, j);
            if (item && item->text().contains(text, Qt::CaseInsensitive)) {
                match = true;
                break;
            }
        }
        trackTableView->setRowHidden(i, !match);
    }
}
