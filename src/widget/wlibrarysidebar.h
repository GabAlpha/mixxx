#pragma once

#include <QBasicTimer>
#include <QContextMenuEvent>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QEvent>
#include <QKeyEvent>
#include <QModelIndex>
#include <QPoint>
#include <QTimerEvent>
#include <QTreeView>

#include "library/library_decl.h"
#include "widget/wbasewidget.h"

class WLibrarySidebar : public QTreeView, public WBaseWidget {
    Q_OBJECT
  public:
    explicit WLibrarySidebar(QWidget* parent = nullptr);

    void contextMenuEvent(QContextMenuEvent * event) override;
    void dragMoveEvent(QDragMoveEvent * event) override;
    void dragEnterEvent(QDragEnterEvent * event) override;
    void dropEvent(QDropEvent * event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void focusInEvent(QFocusEvent* event) override;
    void timerEvent(QTimerEvent* event) override;
    void toggleSelectedItem();
    bool isLeafNodeSelected();

  public slots:
    void selectIndex(const QModelIndex&);
    void selectChildIndex(const QModelIndex&, bool selectItem = true);
    void slotSetFont(const QFont& font);

  signals:
    void rightClicked(const QPoint&, const QModelIndex&);
    void renameItem(const QModelIndex&);
    void deleteItem(const QModelIndex&);
    FocusWidget setLibraryFocus(FocusWidget newFocus);

  protected:
    bool event(QEvent* pEvent) override;

  private:
    void focusSelectedIndex();

    QBasicTimer m_expandTimer;
    QModelIndex m_hoverIndex;
};
