import re
import sys
import random
from Xlib import X, display, Xutil
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
        self.setWindowFlags(QtCore.Qt.FramelessWindowHint)
        self.setWindowFlags(QtCore.Qt.WindowStaysOnTopHint)

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
# pos : dict { x: 0, y: 0 }
# returns: QScreen
def getScreenAtPos(screens, pos):
    for screen in screens:
        x_high = screen.geometry().x() + screen.geometry().width()
        x_low = screen.geometry().x()
        y_high = screen.geometry().y() + screen.geometry().height()
        y_low = screen.geometry().y()
        if (x_low <= pos.x < x_high) and (y_low <= pos.y < y_high):
            return screen


if __name__ == "__main__":
    if (sys.stdin.isatty()):
        sys.exit()
    piped_input = sys.stdin.read().splitlines()

    app = QtWidgets.QApplication([])

    # Needed because Qt doesn't know where the Window Manager
    # will create the window before it is painted
    input_focus_geometry = display.Display().get_input_focus().focus.get_geometry()
    current_screen = getScreenAtPos(app.screens(), input_focus_geometry)

    app.setApplicationName("dmenu")
    app.setFont(QtGui.QFont("Menlo for Powerline", 14, QtGui.QFont.Bold))
    widget = Dmenu(piped_input)
    widget.resize(600, 300)
    widget.setFixedSize(600, 300)
    widget.show()
    widget.move(
        current_screen.geometry().width() / 2 - widget.width() / 2,
        current_screen.geometry().height() / 2 - widget.height() / 2
    )
    sys.exit(app.exec_())
