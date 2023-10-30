# Inspired by https://qtdocs.pyvista.org/usage.html

import sys

# Setting the Qt bindings for QtPy
import os
os.environ["QT_API"] = "pyqt6"

from qtpy import QtWidgets

import numpy as np

import pyvista as pv
from pyvista import examples
from pyvistaqt import QtInteractor, MainWindow

class MyMainWindow(MainWindow):

    def __init__(self, parent=None, show=True):
        QtWidgets.QMainWindow.__init__(self, parent)

        # create the frame
        self.frame = QtWidgets.QFrame()
        vlayout = QtWidgets.QVBoxLayout()

        # add the pyvista interactor object
        self.plotter = QtInteractor(self.frame)
        vlayout.addWidget(self.plotter.interactor)
        self.signal_close.connect(self.plotter.close)

        self.frame.setLayout(vlayout)
        self.setCentralWidget(self.frame)

        # simple menu to demo functions
        mainMenu = self.menuBar()
        fileMenu = mainMenu.addMenu('File')
        exitButton = QtWidgets.QAction('Exit', self)
        exitButton.setShortcut('Ctrl+Q')
        exitButton.triggered.connect(self.close)
        fileMenu.addAction(exitButton)

        # allow adding a sphere
        meshMenu = mainMenu.addMenu('Mesh')
        self.add_topo_action = QtWidgets.QAction('Add Topo', self)
        self.add_topo_action.triggered.connect(self.add_topo)
        meshMenu.addAction(self.add_topo_action)

        if show:
            self.show()

    def add_topo(self):
    	# Load the elevation data as a surface
        elevation = examples.download_crater_topo().warp_by_scalar()
        # Load the topographic map from a GeoTiff
        topo_map = examples.download_crater_imagery()
        topo_map = topo_map.flip_y()  # flip to align to our dataset

        # Bounds of the aerial imagery - given to us
        bounds = (1818000, 1824500, 5645000, 5652500, 0, 3000)
        # Clip the elevation dataset to the map's extent
        local = elevation.clip_box(bounds, invert=False)
        # Apply texturing coordinates to associate the image to the surface
        local.texture_map_to_plane(use_bounds=True, inplace=True)

        # Extract surrounding region from elevation data
        surrounding = elevation.clip_box(bounds, invert=True)

        # Display with a shading technique
        self.plotter.add_mesh(local, texture=topo_map)
        self.plotter.add_mesh(surrounding, color="white")
        self.plotter.enable_eye_dome_lighting()
        self.plotter.camera_position = [
            (1831100.0, 5642142.0, 8168.0),
            (1820841.0, 5648745.0, 1104.0),
            (-0.435, 0.248, 0.865),
        ]

window = MyMainWindow(main_window)


