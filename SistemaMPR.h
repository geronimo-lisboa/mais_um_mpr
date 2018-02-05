#pragma once
#include "stdafx.h"
#include "myResliceInteractionStyle.h"
//---------------------------------------------------------
class TelaMPR {
private:
	int id;
	vtkSmartPointer<vtkOpenGLRenderer> rendererLayerWidget;
	vtkSmartPointer<vtkOpenGLRenderer> rendererLayerImage;
	vtkSmartPointer<vtkWin32OpenGLRenderWindow> renderWindow;
	vtkSmartPointer<vtkWin32RenderWindowInteractor> interactor;
	vtkSmartPointer<myResliceInteractionStyle> style;

	vtkSmartPointer<vtkCubeSource> planeSource1, planeSource2, planeSource3;
	vtkSmartPointer<vtkPolyDataMapper> planeSourceMapper1, planeSourceMapper2, planeSourceMapper3;
	vtkSmartPointer<vtkActor> planeActor1, planeActor2, planeActor3;
public:
	TelaMPR(HWND handle, int id);
	void Resize(int w, int h);
	void CreateThings(vtkImageImport* image);
	int GetId() { return id; }
	void OrientPlanes(std::array<double, 3> u, std::array<double, 3> v);
};
//----------------------------------------------------------------------
class SistemaMPR {
private:
	std::array<double, 3> tela01U, tela01V;
	std::array<double, 3> tela02U, tela02V;
	std::array<double, 3> tela03U, tela03V;
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