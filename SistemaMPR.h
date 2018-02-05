#pragma once
#include "stdafx.h"
#include "myResliceInteractionStyle.h"
class SistemaMPR;

class TelaMPR {
private:
	const int id;
	vtkSmartPointer<vtkOpenGLRenderer> rendererLayerWidget;
	vtkSmartPointer<vtkOpenGLRenderer> rendererLayerImage;
	vtkSmartPointer<vtkWin32OpenGLRenderWindow> renderWindow;
	vtkSmartPointer<vtkWin32RenderWindowInteractor> interactor;
	vtkSmartPointer<myResliceInteractionStyle> style;

	vtkSmartPointer<vtkCubeSource> planeSource1, planeSource2, planeSource3;
	vtkSmartPointer<vtkPolyDataMapper> planeSourceMapper1, planeSourceMapper2, planeSourceMapper3;
	vtkSmartPointer<vtkActor> planeActor1, planeActor2, planeActor3;
	SistemaMPR *sistema;
public:
	TelaMPR(HWND handle, SistemaMPR *sistema, int _id);
	void Resize(int w, int h);
	void CreateThings(vtkImageImport* image);
	void SetVectors(std::array<double, 3> vec1, std::array<double, 3> vec2);
};
//----------------------------------------------------------------------
class SistemaMPR {
private:
	std::string idExame, idSerie;
	std::array<std::shared_ptr<TelaMPR>, 3> telasDoMpr;
	itk::Image<short, 3>::Pointer imagem;
	vtkImageImport * vtkImage;
	vtkSmartPointer<vtkMatrix4x4> uvw;
	bool AreThingReadyToCreateMPR();
	void SetUp();
	void UpdatePlanes();
public:
	SistemaMPR(char* idExame, char* idSerie);
	void CreateRenderers(HWND handleTela01, HWND handleTela02, HWND handleTela03);
	void SetImage(itk::Image<short, 3>::Pointer img, vtkImageImport* vtkImage);
	void Resize(int w, int h, int qual);
	void SetMatrix(vtkSmartPointer<vtkMatrix4x4> m);
};