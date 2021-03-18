import sys
import random
from PySide6 import QtCore, QtWidgets, QtGui

class MyWidget(QtWidgets.QWidget):
    def __init__(self):
        super().__init__()

        self.hello = ["Hallo Welt", "Hei maailma", "Hola Mundo", "Привет мир"]

        # self.button = QtWidgets.QPushButton("Click me!")
        # self.text = QtWidgets.QLabel("Hello World",
        #                              alignment=QtCore.Qt.AlignCenter)
        self.tt = QtWidgets.QLineEdit()
        self.setStyleSheet("* { background-color: #484848; color: white; font-weight: bold; font-family: consolas; font-size: 14px; }")
        self.tt.setStyleSheet("border: none")

        self.layout = QtWidgets.QVBoxLayout(self)
        # self.layout.addWidget(self.text)
        # self.layout.addWidget(self.button)
        self.layout.addWidget(self.tt)
        self.setWindowFlags(QtCore.Qt.FramelessWindowHint)

        # self.button.clicked.connect(self.magic)

    @QtCore.Slot()
    def magic(self):
        self.text.setText(random.choice(self.hello))

    def addOption(self, txt):
        self.layout.addWidget(QtWidgets.QLabel(txt, alignment=QtCore.Qt.AlignLeft))

    def keyPressEvent(self, e):
        if e.key() == QtCore.Qt.Key_Escape:
            self.close()


if __name__ == "__main__":
    app = QtWidgets.QApplication([])

    widget = MyWidget()
    widget.resize(600, 40)
    widget.show()
    # print(repr(sys.stdin.readlines()))
    for line in sys.stdin.read().splitlines():
        print(repr(line))
        widget.addOption(line)
        # widget.layout.addWidget(QtWidgets.QLabel(line, alignment=QtCore.Qt.AlignLeft))
        # widget.setStyleSheet("QLabel { border-style: solid; border-width: 2px; border-color: red; }")
    sys.exit(app.exec_())
