#include "selecttrackdialog.h"
#include "track.h"
#include <QHeaderView>

SelectTrackDialog::SelectTrackDialog(const std::vector<Track> &tracks, QWidget *parent)
    : QDialog(parent), selectedTrack(), allTracks(tracks) {
    QVBoxLayout *layout = new QVBoxLayout(this);

    searchBar = new QLineEdit(this);
    searchBar->setPlaceholderText(tr("Search..."));
    layout->addWidget(searchBar);

    trackTableView = new QTableView(this);
    model = new QStandardItemModel(this);
    model->setHorizontalHeaderLabels({"#", "Title", "Artist", "Duration"});
    trackTableView->setModel(model);
    trackTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    trackTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    trackTableView->horizontalHeader()->setStretchLastSection(true);
    trackTableView->verticalHeader()->setVisible(false);
    trackTableView->setEditTriggers(QAbstractItemView::NoEditTriggers); // Disable editing
    layout->addWidget(trackTableView);

    selectButton = new QPushButton(tr("Select"), this);
    layout->addWidget(selectButton);

    connect(selectButton, &QPushButton::clicked, this, &SelectTrackDialog::onTrackSelected);
    connect(searchBar, &QLineEdit::textChanged, this, &SelectTrackDialog::onSearchTextChanged);

    // Populate the table with tracks
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

    // Set a fixed width for the dialog
    setFixedWidth(800);
}

Track SelectTrackDialog::getSelectedTrack() const {
    return selectedTrack;
}

void SelectTrackDialog::onTrackSelected() {
    QModelIndexList selectedRows = trackTableView->selectionModel()->selectedRows();
    if (!selectedRows.isEmpty()) {
        QModelIndex index = selectedRows.first();
        selectedTrack = model->item(index.row(), 0)->data(Qt::UserRole).value<Track>();
        accept();
    }
}

void SelectTrackDialog::onSearchTextChanged(const QString &text) {
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
