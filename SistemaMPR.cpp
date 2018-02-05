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

vtkSmartPointer<vtkTransform> CreateInitialTransformU() {
	auto mat1 = vtkSmartPointer<vtkMatrix4x4>::New();
	mat1->Element[0][0] = 1; mat1->Element[0][1] = 0; mat1->Element[0][2] = 0; mat1->Element[0][3] = 0;
	mat1->Element[1][0] = 0; mat1->Element[1][1] = 1; mat1->Element[1][2] = 0; mat1->Element[1][3] = 0;
	mat1->Element[2][0] = 0; mat1->Element[2][1] = 0; mat1->Element[2][2] = 1; mat1->Element[2][3] = 0;
	mat1->Element[3][0] = 0; mat1->Element[3][1] = 0; mat1->Element[3][2] = 0; mat1->Element[3][3] = 1;
	auto trans01 = vtkSmartPointer<vtkTransform>::New();
	trans01->SetMatrix(mat1);
	trans01->Update();
	return trans01;
}

vtkSmartPointer<vtkTransform> CreateInitialTransformV() {
	auto mat3 = vtkSmartPointer<vtkMatrix4x4>::New();
	mat3->Element[0][0] = 0; mat3->Element[0][1] = 1; mat3->Element[0][2] = 0; mat3->Element[0][3] = 0;
	mat3->Element[1][0] = 0; mat3->Element[1][1] = 0; mat3->Element[1][2] = 1; mat3->Element[1][3] = 0;
	mat3->Element[2][0] = 1; mat3->Element[2][1] = 0; mat3->Element[2][2] = 0; mat3->Element[2][3] = 0;
	mat3->Element[3][0] = 0; mat3->Element[3][1] = 0; mat3->Element[3][2] = 0; mat3->Element[3][3] = 1;
	auto trans03 = vtkSmartPointer<vtkTransform>::New();
	trans03->SetMatrix(mat3);
	trans03->Update();
	return trans03;
}

vtkSmartPointer<vtkTransform> CreateInitialTransformW() {
	auto mat2 = vtkSmartPointer<vtkMatrix4x4>::New();
	mat2->Element[0][0] = 0; mat2->Element[0][1] = 0; mat2->Element[0][2] = 1; mat2->Element[0][3] = 0;
	mat2->Element[1][0] = 1; mat2->Element[1][1] = 0; mat2->Element[1][2] = 0; mat2->Element[1][3] = 0;
	mat2->Element[2][0] = 0; mat2->Element[2][1] = 1; mat2->Element[2][2] = 0; mat2->Element[2][3] = 0;
	mat2->Element[3][0] = 0; mat2->Element[3][1] = 0; mat2->Element[3][2] = 0; mat2->Element[3][3] = 1;
	auto trans02 = vtkSmartPointer<vtkTransform>::New();
	trans02->SetMatrix(mat2);
	trans02->Update();
	return trans02;
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
	planeSource1->SetYLength(5);
	planeSource1->SetZLength(100);
	planeSourceMapper1 = vtkSmartPointer<vtkPolyDataMapper>::New();
	planeSourceMapper1->SetInputConnection(planeSource1->GetOutputPort());
	planeActor1 = vtkSmartPointer<vtkActor>::New();
	planeActor1->SetMapper(planeSourceMapper1);
	planeActor1->GetProperty()->SetColor(1, 0, 0);
	planeActor1->SetUserTransform(CreateInitialTransformU());
	rendererLayerWidget->AddActor(planeActor1);
	
	planeSource2 = vtkSmartPointer<vtkCubeSource>::New();
	planeSource2->SetXLength(100);
	planeSource2->SetYLength(5);
	planeSource2->SetZLength(100);
	planeSourceMapper2 = vtkSmartPointer<vtkPolyDataMapper>::New();
	planeSourceMapper2->SetInputConnection(planeSource2->GetOutputPort());
	planeActor2 = vtkSmartPointer<vtkActor>::New();
	planeActor2->SetMapper(planeSourceMapper2);
	planeActor2->GetProperty()->SetColor(0, 1, 0);
	planeActor2->SetUserTransform(CreateInitialTransformV());
	rendererLayerWidget->AddActor(planeActor2);
	
	dummyImagePlane = vtkSmartPointer<vtkCubeSource>::New();
	dummyImagePlane->SetXLength(100);
	dummyImagePlane->SetYLength(5);
	dummyImagePlane->SetZLength(100);
	dummyImagePlaneMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	dummyImagePlaneMapper->SetInputConnection(dummyImagePlane->GetOutputPort());
	dummyActor = vtkSmartPointer<vtkActor>::New();
	dummyActor->SetMapper(dummyImagePlaneMapper);
	dummyActor->GetProperty()->SetColor(0, 0, 1);
	dummyActor->PickableOff();
	dummyActor->SetUserTransform(CreateInitialTransformW());
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
	style = vtkSmartPointer<myResliceInteractionStyle>::New();
	style->SetOperacao(0, VTKIS_SPIN);
	style->SetOperacao(1, VTKIS_SPIN);
	style->SetOperacao(2, VTKIS_SPIN);
	//interactor->SetInteractorStyle(style);


	renderWindow->Render();
}

void TelaMPR::Resize(int w, int h)
{
	renderWindow->SetSize(w, h);
}
