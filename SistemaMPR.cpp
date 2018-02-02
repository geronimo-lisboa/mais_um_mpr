#include "stdafx.h"
#include "SistemaMPR.h"

bool SistemaMPR::AreThingReadyToCreateMPR()
{
	for (std::shared_ptr<TelaMPR> t : telasDoMpr)
		if (!t) 
			return false;
	if (!imagem)
		return false;
	return true;
}

void SistemaMPR::SetUp()
{
	if (AreThingReadyToCreateMPR()) {

	}
}

SistemaMPR::SistemaMPR(char * idExame, char * idSerie)
{
	this->idExame = idExame;
	this->idSerie = idSerie;
	imagem = nullptr;
}

void SistemaMPR::CreateRenderers(HWND handleTela01, HWND handleTela02, HWND handleTela03)
{
	telasDoMpr[0] = std::make_shared<TelaMPR>(handleTela01);
	telasDoMpr[1] = std::make_shared<TelaMPR>(handleTela02);
	telasDoMpr[2] = std::make_shared<TelaMPR>(handleTela03);
	SetUp();
}

void SistemaMPR::SetImage(itk::Image<short, 3>::Pointer img)
{
	imagem = img;
	SetUp();
}

void SistemaMPR::Resize(int w, int h, int qual)
{
	telasDoMpr[qual]->Resize(w, h);
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
	//Criação de um plano pra testar como fica
	planeSource = vtkSmartPointer<vtkCubeSource>::New();
	planeSourceMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	planeSourceMapper->SetInputConnection(planeSource->GetOutputPort());
	planeActor = vtkSmartPointer<vtkActor>::New();
	planeActor->SetMapper(planeSourceMapper);
	rendererLayerWidget->AddActor(planeActor);
	rendererLayerWidget->ResetCamera();

	renderWindow->Render();
}

void TelaMPR::Resize(int w, int h)
{
	renderWindow->SetSize(w, h);
}
