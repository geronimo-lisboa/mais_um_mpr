#pragma once
#include "stdafx.h"
#include "myResliceInteractionStyle.h"
class TelaMPR {
private:
	vtkSmartPointer<vtkOpenGLRenderer> rendererLayerWidget;
	vtkSmartPointer<vtkOpenGLRenderer> rendererLayerImage;
	vtkSmartPointer<vtkWin32OpenGLRenderWindow> renderWindow;
	vtkSmartPointer<vtkWin32RenderWindowInteractor> interactor;
	vtkSmartPointer<myResliceInteractionStyle> style;

	vtkSmartPointer<vtkCubeSource> planeSource1, planeSource2, dummyImagePlane;
	vtkSmartPointer<vtkPolyDataMapper> planeSourceMapper1, planeSourceMapper2, dummyImagePlaneMapper;
	vtkSmartPointer<vtkActor> planeActor1, planeActor2, dummyActor;
public:
	TelaMPR(HWND handle);
	void Resize(int w, int h);
	void CreateThings(vtkImageImport* image);
};
//----------------------------------------------------------------------
class SistemaMPR {
private:
	std::string idExame, idSerie;
	std::array<std::shared_ptr<TelaMPR>, 3> telasDoMpr;
	itk::Image<short, 3>::Pointer imagem;
	vtkImageImport * vtkImage;
	bool AreThingReadyToCreateMPR();
	void SetUp();
public:
	SistemaMPR(char* idExame, char* idSerie);
	void CreateRenderers(HWND handleTela01, HWND handleTela02, HWND handleTela03);
	void SetImage(itk::Image<short, 3>::Pointer img, vtkImageImport* vtkImage);
	void Resize(int w, int h, int qual);
};