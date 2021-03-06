#ifndef VMDEDITOR_H
#define VMDEDITOR_H

#include <QVector>
#include <QString>
#include <QClipboard>
#include <QImage>

#include "vtextedit.h"
#include "veditor.h"
#include "vconfigmanager.h"
#include "vtableofcontent.h"
#include "vconfigmanager.h"
#include "utils/vutils.h"

class HGMarkdownHighlighter;
class VCodeBlockHighlightHelper;
class VDocument;
class VPreviewManager;

class VMdEditor : public VTextEdit, public VEditor
{
    Q_OBJECT
public:
    VMdEditor(VFile *p_file,
              VDocument *p_doc,
              MarkdownConverterType p_type,
              QWidget *p_parent = nullptr);

    void beginEdit() Q_DECL_OVERRIDE;

    void endEdit() Q_DECL_OVERRIDE;

    void saveFile() Q_DECL_OVERRIDE;

    void reloadFile() Q_DECL_OVERRIDE;

    bool scrollToBlock(int p_blockNumber) Q_DECL_OVERRIDE;

    void makeBlockVisible(const QTextBlock &p_block) Q_DECL_OVERRIDE;

    QVariant inputMethodQuery(Qt::InputMethodQuery p_query) const Q_DECL_OVERRIDE;

    bool isBlockVisible(const QTextBlock &p_block) Q_DECL_OVERRIDE;

    // An image has been inserted. The image is relative.
    // @p_path is the absolute path of the inserted image.
    void imageInserted(const QString &p_path);

    // Scroll to header @p_blockNumber.
    // Return true if @p_blockNumber is valid to scroll to.
    bool scrollToHeader(int p_blockNumber);

    void scrollBlockInPage(int p_blockNum, int p_dest) Q_DECL_OVERRIDE;

    void updateConfig() Q_DECL_OVERRIDE;

    QString getContent() const Q_DECL_OVERRIDE;

    void setContent(const QString &p_content, bool p_modified = false) Q_DECL_OVERRIDE;

public slots:
    bool jumpTitle(bool p_forward, int p_relativeLevel, int p_repeat) Q_DECL_OVERRIDE;

// Wrapper functions for QPlainTextEdit/QTextEdit.
public:
    void setExtraSelectionsW(const QList<QTextEdit::ExtraSelection> &p_selections) Q_DECL_OVERRIDE
    {
        setExtraSelections(p_selections);
    }

    QTextDocument *documentW() const Q_DECL_OVERRIDE
    {
        return document();
    }

    void setTabStopWidthW(int p_width) Q_DECL_OVERRIDE
    {
        setTabStopWidth(p_width);
    }

    QTextCursor textCursorW() const Q_DECL_OVERRIDE
    {
        return textCursor();
    }

    void moveCursorW(QTextCursor::MoveOperation p_operation,
                     QTextCursor::MoveMode p_mode) Q_DECL_OVERRIDE
    {
        moveCursor(p_operation, p_mode);
    }

    QScrollBar *verticalScrollBarW() const Q_DECL_OVERRIDE
    {
        return verticalScrollBar();
    }

    QScrollBar *horizontalScrollBarW() const Q_DECL_OVERRIDE
    {
        return horizontalScrollBar();
    }

    void setTextCursorW(const QTextCursor &p_cursor) Q_DECL_OVERRIDE
    {
        setTextCursor(p_cursor);
    }

    bool findW(const QString &p_exp,
               QTextDocument::FindFlags p_options = QTextDocument::FindFlags()) Q_DECL_OVERRIDE
    {
        return find(p_exp, p_options);
    }

    bool findW(const QRegExp &p_exp,
               QTextDocument::FindFlags p_options = QTextDocument::FindFlags()) Q_DECL_OVERRIDE
    {
        return find(p_exp, p_options);
    }

    void setReadOnlyW(bool p_ro) Q_DECL_OVERRIDE
    {
        setReadOnly(p_ro);
    }

    QWidget *viewportW() const Q_DECL_OVERRIDE
    {
        return viewport();
    }

    void insertPlainTextW(const QString &p_text) Q_DECL_OVERRIDE
    {
        insertPlainText(p_text);
    }

    void undoW() Q_DECL_OVERRIDE
    {
        undo();
    }

    void redoW() Q_DECL_OVERRIDE
    {
        redo();
    }

signals:
    // Signal when headers change.
    void headersChanged(const QVector<VTableOfContentItem> &p_headers);

    // Signal when current header change.
    void currentHeaderChanged(int p_blockNumber);

    // Signal when the status of VMdEdit changed.
    // Will be emitted by VImagePreviewer for now.
    void statusChanged();

protected:
    void updateFontAndPalette() Q_DECL_OVERRIDE;

    void contextMenuEvent(QContextMenuEvent *p_event) Q_DECL_OVERRIDE;

    // Used to implement dragging mouse with Ctrl and left button pressed to scroll.
    void mousePressEvent(QMouseEvent *p_event) Q_DECL_OVERRIDE;

    void mouseReleaseEvent(QMouseEvent *p_event) Q_DECL_OVERRIDE;

    void mouseMoveEvent(QMouseEvent *p_event) Q_DECL_OVERRIDE;

    void keyPressEvent(QKeyEvent *p_event) Q_DECL_OVERRIDE;

    bool canInsertFromMimeData(const QMimeData *p_source) const Q_DECL_OVERRIDE;

    void insertFromMimeData(const QMimeData *p_source) Q_DECL_OVERRIDE;

private slots:
    // Update m_headers according to elements.
    void updateHeaders(const QVector<VElementRegion> &p_headerRegions);

    // Update current header according to cursor position.
    // When there is no header in current cursor, will signal an invalid header.
    void updateCurrentHeader();

private:
    // Update the config of VTextEdit according to global configurations.
    void updateTextEditConfig();

    // Get the initial images from file before edit.
    void initInitImages();

    // Clear two kind of images according to initial images and current images:
    // 1. Newly inserted images which are deleted later;
    // 2. Initial images which are deleted;
    void clearUnusedImages();

    // Index in m_headers of current header which contains the cursor.
    int indexOfCurrentHeader() const;

    HGMarkdownHighlighter *m_mdHighlighter;

    VCodeBlockHighlightHelper *m_cbHighlighter;

    VPreviewManager *m_previewMgr;

    // Image links inserted while editing.
    QVector<ImageLink> m_insertedImages;

    // Image links right at the beginning of the edit.
    QVector<ImageLink> m_initImages;

    // Mainly used for title jump.
    QVector<VTableOfContentItem> m_headers;

    bool m_freshEdit;
};

#endif // VMDEDITOR_H
