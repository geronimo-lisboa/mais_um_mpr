#pragma once
#include "stdafx.h"

class TelaMPR {
private:
	vtkSmartPointer<vtkOpenGLRenderer> rendererLayerWidget;
	vtkSmartPointer<vtkOpenGLRenderer> rendererLayerImage;
	vtkSmartPointer<vtkWin32OpenGLRenderWindow> renderWindow;
	vtkSmartPointer<vtkWin32RenderWindowInteractor> interactor;
	vtkSmartPointer<vtkInteractorStyleTrackballActor> style;

	vtkSmartPointer<vtkCubeSource> planeSource;
	vtkSmartPointer<vtkPolyDataMapper> planeSourceMapper;
	vtkSmartPointer<vtkActor> planeActor;
public:
	TelaMPR(HWND handle);
	void Resize(int w, int h);
};

class SistemaMPR {
private:
	std::string idExame, idSerie;
	std::array<std::shared_ptr<TelaMPR>, 3> telasDoMpr;
	itk::Image<short, 3>::Pointer imagem;
	bool AreThingReadyToCreateMPR();
	void SetUp();
public:
	SistemaMPR(char* idExame, char* idSerie);
	void CreateRenderers(HWND handleTela01, HWND handleTela02, HWND handleTela03);
	void SetImage(itk::Image<short, 3>::Pointer img);
	void Resize(int w, int h, int qual);
};