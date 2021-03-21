import re
import sys
import random
from PySide6 import QtCore, QtWidgets, QtGui


class Dmenu(QtWidgets.QFrame):
    def __init__(self, data):
        super().__init__()
        self.isLoaded = False
        with open("css/dmenu.css", "r") as dmenuStyle:
            self.setStyleSheet(dmenuStyle.read())
        self.textbox = QtWidgets.QLineEdit()
        self.textbox.textChanged[str].connect(self.onChanged)
        self.menu = QtWidgets.QListWidget()
        self.items = data
        self.menu.addItems(data)
        self.menu.setCurrentRow(0)
        self.layout = QtWidgets.QVBoxLayout(self)
        self.layout.setContentsMargins(0, 0, 0, 0)
        self.layout.addWidget(self.textbox)
        self.layout.addWidget(self.menu)
        self.setWindowFlags(QtCore.Qt.FramelessWindowHint | QtCore.Qt.WindowStaysOnTopHint)

    def keyPressEvent(self, e):
        if e.key() == QtCore.Qt.Key_Escape:
            self.close()
        elif e.key() == QtCore.Qt.Key_Down:
            if (self.menu.currentRow() == self.menu.count() - 1):
                return
            self.menu.setCurrentRow(self.menu.currentRow() + 1)
        elif e.key() == QtCore.Qt.Key_Up:
            if (self.menu.currentRow() == 0):
                return
            self.menu.setCurrentRow(self.menu.currentRow() - 1)
        elif e.key() == QtCore.Qt.Key_Return:
            print(self.menu.currentItem().text())
            self.close()

    def onChanged(self, text):
        p = re.compile(f".*{text}.*")
        filtered_data = [s for s in self.items if p.match(s)]
        self.menu.clear()
        self.menu.addItems(filtered_data)
        self.menu.setCurrentRow(0)


# screens : QScreensList
# pos : QPoint
# returns: QScreen
def getScreenAtPos(screens, pos):
    for screen in screens:
        x_high = screen.geometry().x() + screen.geometry().width()
        x_low = screen.geometry().x()
        y_high = screen.geometry().y() + screen.geometry().height()
        y_low = screen.geometry().y()
        if (x_low <= pos.x() < x_high) and (y_low <= pos.y() < y_high):
            return screen


if __name__ == "__main__":
    top = None
    left = None

    if (sys.stdin.isatty()):
        sys.exit()
    piped_input = sys.stdin.read().splitlines()

    app = QtWidgets.QApplication([])
    app.setApplicationName("dmenu")
    app.setFont(QtGui.QFont("Menlo for Powerline", 14, QtGui.QFont.Bold))
    widget = Dmenu(piped_input)

    if sys.platform.startswith('linux'):
        # Needed because Qt doesn't know on which monitor the Window Manager will
        # place the window before it is painted. We get the coordinates of the
        # window that has input focus and find the monitor on which it is drawn.
        from Xlib import X, display, Xutil
        ifocus = display.Display().get_input_focus()
        ifocus_geometry = ifocus.focus.get_geometry()
        curr_pos = QtCore.QPoint(ifocus_geometry.x, ifocus_geometry.y)
        # For some reason, top position is relative to the monitor
        # and not to the screen's root window, unlike left position.
        top = 0
    elif sys.platform.startswith('win32'):
        # On windows, widget is placed on default screen so we use the cursor's
        # position to decide on which screen to place the widget.
        curr_pos = QtGui.QCursor.pos()
    elif sys.platform.startswith('freebsd'):
        print("Operating system not yet supported")
        sys.exit()
    elif sys.platform.startswith('darwin'):
        print("Operating system not yet supported")
        sys.exit()
    else:
        print("Maybe you should reconsider your life choices.")
        print("jk, submit a patch if you need this tool on your OS")
        sys.exit()

    current_screen = getScreenAtPos(app.screens(), curr_pos)
    widget.resize(600, 300)
    widget.setFixedSize(600, 300)
    widget.show()
    widget.setScreen(current_screen)
    if (top is None):
        top = current_screen.geometry().y()
    if (left is None):
        left = current_screen.geometry().x()
    widget.move(
        left + current_screen.geometry().width() / 2 - widget.width() / 2,
        top + current_screen.geometry().height() / 2 - widget.height() / 2
    )
    sys.exit(app.exec_())
