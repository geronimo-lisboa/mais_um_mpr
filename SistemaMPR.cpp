#include "stdafx.h"
#include "SistemaMPR.h"

bool SistemaMPR::AreThingReadyToCreateMPR()
{
	for (std::shared_ptr<TelaMPR> t : telasDoMpr)
		if (!t) 
			return false;
	if (!imagem)
		return false;
	if (!vtkImage)
		return false;
	return true;
}

void SistemaMPR::SetUp()
{
	if (AreThingReadyToCreateMPR()) {
		for (std::shared_ptr<TelaMPR> t : telasDoMpr)
			t->CreateThings(vtkImage);
	}
}

SistemaMPR::SistemaMPR(char * idExame, char * idSerie)
{
	this->idExame = idExame;
	this->idSerie = idSerie;
	imagem = nullptr;
	vtkImage = nullptr;
}

void SistemaMPR::CreateRenderers(HWND handleTela01, HWND handleTela02, HWND handleTela03)
{
	telasDoMpr[0] = std::make_shared<TelaMPR>(handleTela01);
	telasDoMpr[1] = std::make_shared<TelaMPR>(handleTela02);
	telasDoMpr[2] = std::make_shared<TelaMPR>(handleTela03);
	SetUp();
}

void SistemaMPR::SetImage(itk::Image<short, 3>::Pointer img, vtkImageImport* vtkImage)
{
	imagem = img;
	this->vtkImage = vtkImage;
	SetUp();
}

void SistemaMPR::Resize(int w, int h, int qual)
{
	telasDoMpr[qual]->Resize(w, h);
}

void TelaMPR::CreateThings(vtkImageImport* image)
{
	//Criação de um plano pra testar como fica
	planeSource1 = vtkSmartPointer<vtkCubeSource>::New();
	planeSource1->SetXLength(100);
	planeSource1->SetZLength(100);
	planeSource1->SetYLength(1);
	planeSourceMapper1 = vtkSmartPointer<vtkPolyDataMapper>::New();
	planeSourceMapper1->SetInputConnection(planeSource1->GetOutputPort());
	planeActor1 = vtkSmartPointer<vtkActor>::New();
	planeActor1->SetMapper(planeSourceMapper1);
	planeActor1->GetProperty()->SetColor(1, 0, 0);
	rendererLayerWidget->AddActor(planeActor1);


	planeSource2 = vtkSmartPointer<vtkCubeSource>::New();
	planeSource2->SetXLength(1);
	planeSource2->SetZLength(100);
	planeSource2->SetYLength(100);
	planeSourceMapper2 = vtkSmartPointer<vtkPolyDataMapper>::New();
	planeSourceMapper2->SetInputConnection(planeSource2->GetOutputPort());
	planeActor2 = vtkSmartPointer<vtkActor>::New();
	planeActor2->SetMapper(planeSourceMapper2);
	planeActor2->GetProperty()->SetColor(0, 1, 0);
	rendererLayerWidget->AddActor(planeActor2);

	dummyImagePlane = vtkSmartPointer<vtkCubeSource>::New();
	dummyImagePlane->SetXLength(100);
	dummyImagePlane->SetZLength(1);
	dummyImagePlane->SetYLength(100);
	dummyImagePlaneMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	dummyImagePlaneMapper->SetInputConnection(dummyImagePlane->GetOutputPort());
	dummyActor = vtkSmartPointer<vtkActor>::New();
	dummyActor->SetMapper(dummyImagePlaneMapper);
	dummyActor->GetProperty()->SetColor(0, 0, 1);
	dummyActor->PickableOff();

	rendererLayerWidget->AddActor(dummyActor);

	rendererLayerWidget->ResetCamera();
}

TelaMPR::TelaMPR(HWND handle)
{
	//Criação das camadas e da tela.
	rendererLayerImage = vtkSmartPointer<vtkOpenGLRenderer>::New();
	rendererLayerImage->GetActiveCamera()->ParallelProjectionOn();
	rendererLayerImage->SetBackground(0.0, 0.2, 0.0);
	rendererLayerImage->ResetCamera();
	rendererLayerImage->GetActiveCamera()->Zoom(1.5);
	rendererLayerImage->SetLayer(0);
	rendererLayerWidget = vtkSmartPointer<vtkOpenGLRenderer>::New();
	rendererLayerWidget->GetActiveCamera()->ParallelProjectionOn();
	rendererLayerWidget->SetBackground(0.1, 0.2, 0.4);
	rendererLayerWidget->ResetCamera();
	rendererLayerWidget->GetActiveCamera()->Zoom(1.5);
	rendererLayerWidget->SetLayer(1);
	renderWindow = vtkSmartPointer<vtkWin32OpenGLRenderWindow>::New();
	renderWindow->SetParentId(handle);
	renderWindow->SetNumberOfLayers(2);
	renderWindow->SetSize(500, 500);
	renderWindow->SetPosition(500, 0);
	renderWindow->AddRenderer(rendererLayerImage);
	renderWindow->AddRenderer(rendererLayerWidget);
	interactor = vtkSmartPointer<vtkWin32RenderWindowInteractor>::New();
	interactor->SetRenderWindow(renderWindow);
	interactor->InstallMessageProcOn();
	style = vtkSmartPointer<vtkInteractorStyleTrackballActor>::New();
	interactor->SetInteractorStyle(style);


	renderWindow->Render();
}

void TelaMPR::Resize(int w, int h)
{
	renderWindow->SetSize(w, h);
}
