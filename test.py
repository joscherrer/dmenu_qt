import sys
import random
from PySide6 import QtCore, QtWidgets, QtGui

class Dmenu(QtWidgets.QWidget):
    def __init__(self):
        super().__init__()
        self.tt = QtWidgets.QLineEdit()
        self.setStyleSheet("* { background-color: #484848; color: white; font-weight: bold; font-family: consolas; font-size: 14px; margin: 0px; padding: 0px; }")
        self.tt.setStyleSheet("border: none")
        self.items = [None]
        self.selected = 0
        self.layout = QtWidgets.QVBoxLayout(self)
        self.layout.setContentsMargins(0,0,0,0)
        # self.layout.addWidget(self.text)
        # self.layout.addWidget(self.button)
        self.layout.addWidget(self.tt)
        self.setWindowFlags(QtCore.Qt.FramelessWindowHint)

        # self.button.clicked.connect(self.magic)

    @QtCore.Slot()
    def magic(self):
        self.text.setText(random.choice(self.hello))

    def addOption(self, txt):
        new_label = QtWidgets.QLabel(txt, alignment=QtCore.Qt.AlignLeft)
        self.items.append(new_label)
        self.layout.addWidget(self.items[-1])
    
    def highlight(self, _id):
        if (_id != 0):
            self.items[_id].setStyleSheet("* { background-color: #ababab; color: black; }")
    
    def unlight(self, _id):
        if (_id != 0):
            self.items[_id].setStyleSheet("* { background-color: #484848; color: white; }")


    def keyPressEvent(self, e):
        if e.key() == QtCore.Qt.Key_Escape:
            self.close()
        elif e.key() == QtCore.Qt.Key_Down:
            self.unlight(self.selected)
            self.selected = self.selected + 1
            self.highlight(self.selected)
        elif e.key() == QtCore.Qt.Key_Up:
            self.unlight(self.selected)
            self.selected = self.selected - 1
            self.highlight(self.selected)
        elif e.key() == QtCore.Qt.Key_Return:
            print(self.items[self.selected].text())
            self.close()


if __name__ == "__main__":
    if (sys.stdin.isatty()):
        sys.exit()

    app = QtWidgets.QApplication([])
    widget = Dmenu()
    widget.resize(600, 40)
    widget.show()
    
    piped_input = sys.stdin.read().splitlines()
    for line in piped_input:
        widget.addOption(line)

    sys.exit(app.exec_())
