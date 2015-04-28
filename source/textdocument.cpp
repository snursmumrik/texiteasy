#include "textdocument.h"

#include <QTextBlock>

TextDocument::TextDocument(QObject *parent) : QTextDocument(parent),
    _lastSaveRevision(0)
{
    connect(this, &TextDocument::modificationChanged, [this](bool modified) {
        if(!modified)
        {
            this->updateRevisions();
        }
    });
}

void TextDocument::resetRevisions()
{
    _lastSaveRevision = revision();

    for (QTextBlock block = begin(); block.isValid(); block = block.next())
        block.setRevision(_lastSaveRevision);
}

void TextDocument::updateRevisions()
{
    int oldLastSaveRevision = _lastSaveRevision;
    _lastSaveRevision = revision();

    if (oldLastSaveRevision != _lastSaveRevision) {
        for (QTextBlock block = begin(); block.isValid(); block = block.next()) {
            if (block.revision() < 0 || block.revision() != oldLastSaveRevision)
                block.setRevision(-_lastSaveRevision - 1);
            else
                block.setRevision(_lastSaveRevision);
        }
    }
}