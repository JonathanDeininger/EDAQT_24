#include "selecttrackdialog.h"
#include "track.h"

SelectTrackDialog::SelectTrackDialog(const std::vector<Track> &tracks, QWidget *parent)
    : QDialog(parent), selectedTrack("") {
    QVBoxLayout *layout = new QVBoxLayout(this);

    trackListWidget = new QListWidget(this);
    for (const auto &track : tracks) {
        QListWidgetItem *item = new QListWidgetItem(track.getTitle(), trackListWidget);
        item->setData(Qt::UserRole, track.getFilePath());
    }
    layout->addWidget(trackListWidget);

    selectButton = new QPushButton(tr("Select"), this);
    layout->addWidget(selectButton);

    connect(selectButton, &QPushButton::clicked, this, &SelectTrackDialog::onTrackSelected);
}

QString SelectTrackDialog::getSelectedTrack() const {
    return selectedTrack;
}

void SelectTrackDialog::onTrackSelected() {
    QListWidgetItem *item = trackListWidget->currentItem();
    if (item) {
        selectedTrack = item->data(Qt::UserRole).toString();
        accept();
    }
}