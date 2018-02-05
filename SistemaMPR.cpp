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
	tela01U = { { 1,0,0 } };
	tela01V = { { 0,1,0 } };

	tela02U = { { 0,1,0 } };
	tela02V = { { 0,0,1 } };

	tela03U = { { 0,0,1 } };
	tela03V = { { 1,0,0 } };
}

void SistemaMPR::CreateRenderers(HWND handleTela01, HWND handleTela02, HWND handleTela03)
{
	telasDoMpr[0] = std::make_shared<TelaMPR>(handleTela01,0);
	telasDoMpr[1] = std::make_shared<TelaMPR>(handleTela02,1);
	telasDoMpr[2] = std::make_shared<TelaMPR>(handleTela03,2);
	SetUp();
}

void SistemaMPR::SetImage(itk::Image<short, 3>::Pointer img, vtkImageImport* vtkImage)
{
	imagem = img;
	this->vtkImage = vtkImage;
	SetUp();
	telasDoMpr[0]->OrientPlanes(tela01U, tela01V);
	telasDoMpr[1]->OrientPlanes(tela02U, tela02V);
	telasDoMpr[2]->OrientPlanes(tela03U, tela03V);
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
	
	planeSource3 = vtkSmartPointer<vtkCubeSource>::New();
	planeSource3->SetXLength(100);
	planeSource3->SetYLength(5);
	planeSource3->SetZLength(100);
	planeSourceMapper3 = vtkSmartPointer<vtkPolyDataMapper>::New();
	planeSourceMapper3->SetInputConnection(planeSource3->GetOutputPort());
	planeActor3 = vtkSmartPointer<vtkActor>::New();
	planeActor3->SetMapper(planeSourceMapper3);
	planeActor3->GetProperty()->SetColor(0, 0, 1);
	planeActor3->SetUserTransform(CreateInitialTransformW());
	rendererLayerWidget->AddActor(planeActor3);

	rendererLayerWidget->ResetCamera();
}

void TelaMPR::OrientPlanes(std::array<double, 3> u, std::array<double, 3> v)
{
	std::array<double, 3> w1, w2;
	vtkMath::Cross(u.data(), v.data(), w1.data());
	vtkMath::Cross(v.data(), u.data(), w2.data());

	auto mat1 = vtkSmartPointer<vtkMatrix4x4>::New();
	mat1->Element[0][0] = u[0]; mat1->Element[0][1] = v[0]; mat1->Element[0][2] = w1[0]; mat1->Element[0][3] = 0;
	mat1->Element[1][0] = u[1]; mat1->Element[1][1] = v[1]; mat1->Element[1][2] = w1[1]; mat1->Element[1][3] = 0;
	mat1->Element[2][0] = u[2]; mat1->Element[2][1] = v[2]; mat1->Element[2][2] = w1[2]; mat1->Element[2][3] = 0;
	mat1->Element[3][0] = 0; mat1->Element[3][1] = 0; mat1->Element[3][2] = 0; mat1->Element[3][3] = 1;
	auto trans01 = vtkSmartPointer<vtkTransform>::New();
	trans01->SetMatrix(mat1);
	trans01->Update();

	auto mat2 = vtkSmartPointer<vtkMatrix4x4>::New();
	mat2->Element[0][0] = v[0]; mat2->Element[0][1] = u[0]; mat2->Element[0][2] = w2[0]; mat2->Element[0][3] = 0;
	mat2->Element[1][0] = v[1]; mat2->Element[1][1] = u[1]; mat2->Element[1][2] = w2[1]; mat2->Element[1][3] = 0;
	mat2->Element[2][0] = v[2]; mat2->Element[2][1] = u[2]; mat2->Element[2][2] = w2[2]; mat2->Element[2][3] = 0;
	mat2->Element[3][0] = 0; mat2->Element[3][1] = 0; mat2->Element[3][2] = 0; mat2->Element[3][3] = 1;
	auto trans02 = vtkSmartPointer<vtkTransform>::New();
	trans02->SetMatrix(mat2);
	trans02->Update();

	auto mat3 = vtkSmartPointer<vtkMatrix4x4>::New();
	mat3->Element[0][0] = u[0]; mat3->Element[0][1] = w1[0]; mat3->Element[0][2] = v[0]; mat3->Element[0][3] = 0;
	mat3->Element[1][0] = u[1]; mat3->Element[1][1] = w1[1]; mat3->Element[1][2] = v[1]; mat3->Element[1][3] = 0;
	mat3->Element[2][0] = u[2]; mat3->Element[2][1] = w1[2]; mat3->Element[2][2] = v[2]; mat3->Element[2][3] = 0;
	mat3->Element[3][0] = 0; mat3->Element[3][1] = 0; mat3->Element[3][2] = 0; mat3->Element[3][3] = 1;
	auto trans03 = vtkSmartPointer<vtkTransform>::New();
	trans03->SetMatrix(mat3);
	trans03->Update();

	planeActor1->SetUserTransform(trans01);
	planeActor2->SetUserTransform(trans02);
	planeActor3->SetUserTransform(trans03);
}

TelaMPR::TelaMPR(HWND handle, int id)
{
	this->id = id;
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
	interactor->SetInteractorStyle(style);
 

	renderWindow->Render();
}

void TelaMPR::Resize(int w, int h)
{
	renderWindow->SetSize(w, h);
}
 