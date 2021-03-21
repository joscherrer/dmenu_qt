import re
import sys
import random
from PySide6 import QtCore, QtWidgets, QtGui


class TestWidget(QtWidgets.QWidget):
    def __init__(self):
        super().__init__()
        self.textbox = QtWidgets.QLineEdit()
        self.textbox.textChanged[str].connect(self.onChanged)
        self.layout = QtWidgets.QVBoxLayout(self)
        self.layout.addWidget(self.textbox)
        self.isLoaded = False

    def paintEvent(self, e):
        if (not self.isLoaded):
            self.isLoaded = True
            print(self.screen())
            print(self.screen().geometry())

    def onChanged(self, text):
        print(self.screen())
        print(self.screen().geometry())


if __name__ == "__main__":
    app = QtWidgets.QApplication([])
    app.setApplicationName("dev")
    widget = TestWidget()
    widget.show()
    current_screen = widget.screen()
    # print(app.activeWindow())
    print(widget.screen())
    print(widget.screen().geometry())
    # print(widget.geometry())
    # print(app.primaryScreen())
    # widget.move(0, 0)
    # print(widget.screen())
    # print(widget.screen().geometry())
    app.exec_()
    sys.exit()
