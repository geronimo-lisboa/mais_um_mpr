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
		UpdatePlanes();
	}
}

void SistemaMPR::UpdatePlanes()
{
	std::array<double, 3> u = { { uvw->Element[0][0], uvw->Element[1][0], uvw->Element[2][0] } };
	std::array<double, 3> v = { { uvw->Element[0][1], uvw->Element[1][1], uvw->Element[2][1] } };
	std::array<double, 3> w = { { uvw->Element[0][2], uvw->Element[1][2], uvw->Element[2][2] } };
	telasDoMpr[0]->SetVectors(u, v);
	telasDoMpr[1]->SetVectors(w, u);
	telasDoMpr[2]->SetVectors(v, w);
}

SistemaMPR::SistemaMPR(char * idExame, char * idSerie)
{
	this->idExame = idExame;
	this->idSerie = idSerie;
	imagem = nullptr;
	vtkImage = nullptr;
	uvw = vtkSmartPointer<vtkMatrix4x4>::New();
	uvw->Identity();
}

void SistemaMPR::CreateRenderers(HWND handleTela01, HWND handleTela02, HWND handleTela03)
{
	telasDoMpr[0] = std::make_shared<TelaMPR>(handleTela01, this, 0);
	telasDoMpr[1] = std::make_shared<TelaMPR>(handleTela02, this, 1);
	telasDoMpr[2] = std::make_shared<TelaMPR>(handleTela03, this, 2);
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

void SistemaMPR::SetMatrix(vtkSmartPointer<vtkMatrix4x4> m)
{
	this->uvw = m;
	UpdatePlanes();
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
	planeSource2->SetXLength(100);
	planeSource2->SetZLength(100);
	planeSource2->SetYLength(1);
	planeSourceMapper2 = vtkSmartPointer<vtkPolyDataMapper>::New();
	planeSourceMapper2->SetInputConnection(planeSource2->GetOutputPort());
	planeActor2 = vtkSmartPointer<vtkActor>::New();
	planeActor2->SetMapper(planeSourceMapper2);
	planeActor2->GetProperty()->SetColor(0, 1, 0);
	rendererLayerWidget->AddActor(planeActor2);

	planeSource3 = vtkSmartPointer<vtkCubeSource>::New();
	planeSource3->SetXLength(100);
	planeSource3->SetZLength(100);
	planeSource3->SetYLength(1);
	planeSourceMapper3 = vtkSmartPointer<vtkPolyDataMapper>::New();
	planeSourceMapper3->SetInputConnection(planeSource3->GetOutputPort());
	planeActor3 = vtkSmartPointer<vtkActor>::New();
	planeActor3->SetMapper(planeSourceMapper3);
	planeActor3->GetProperty()->SetColor(0, 0, 1);
	planeActor3->PickableOff();

	rendererLayerWidget->AddActor(planeActor3);

	rendererLayerWidget->ResetCamera();

	style->SetPropsDoReslice(planeActor1, planeActor2);
}

void TelaMPR::SetVectors(std::array<double, 3> vec1, std::array<double, 3> vec2)
{
	std::array<double, 3> vec3;
	vtkMath::Cross(vec1.data(), vec2.data(), vec3.data());
	//aplica ao horizontal
	auto matPlano01 = vtkSmartPointer<vtkMatrix4x4>::New();
	matPlano01->Element[0][0] = vec2[0]; matPlano01->Element[0][1] = vec1[0]; matPlano01->Element[0][2] = vec3[0]; matPlano01->Element[0][3] = 0;
	matPlano01->Element[1][0] = vec2[1]; matPlano01->Element[1][1] = vec1[1]; matPlano01->Element[1][2] = vec3[1]; matPlano01->Element[1][3] = 0;
	matPlano01->Element[2][0] = vec2[2]; matPlano01->Element[2][1] = vec1[2]; matPlano01->Element[2][2] = vec3[2]; matPlano01->Element[2][3] = 0;
	matPlano01->Element[3][0] = 0; matPlano01->Element[3][1] = 0; matPlano01->Element[3][2] = 0; matPlano01->Element[3][3] = 1;
	auto newTransform01 = vtkSmartPointer<vtkTransform>::New();
	newTransform01->SetMatrix(matPlano01);
	newTransform01->Update();
	planeActor1->SetUserTransform(newTransform01);
	planeActor1->GetProperty()->LightingOff();
	//aplica ao vertical
	auto matPlano02 = vtkSmartPointer<vtkMatrix4x4>::New();
	matPlano02->Element[0][0] = vec1[0]; matPlano02->Element[0][1] = vec2[0]; matPlano02->Element[0][2] = vec3[0]; matPlano02->Element[0][3] = 0;
	matPlano02->Element[1][0] = vec1[1]; matPlano02->Element[1][1] = vec2[1]; matPlano02->Element[1][2] = vec3[1]; matPlano02->Element[1][3] = 0;
	matPlano02->Element[2][0] = vec1[2]; matPlano02->Element[2][1] = vec2[2]; matPlano02->Element[2][2] = vec3[2]; matPlano02->Element[2][3] = 0;
	matPlano02->Element[3][0] = 0; matPlano02->Element[3][1] = 0; matPlano02->Element[3][2] = 0; matPlano02->Element[3][3] = 1;
	auto newTransform02 = vtkSmartPointer<vtkTransform>::New();
	newTransform02->SetMatrix(matPlano02);
	newTransform02->Update();
	planeActor2->SetUserTransform(newTransform02);
	planeActor2->GetProperty()->LightingOff();
	//aplica ao do fundo
	auto matPlano03 = vtkSmartPointer<vtkMatrix4x4>::New();
	matPlano03->Element[0][0] = vec1[0]; matPlano03->Element[0][1] = vec3[0]; matPlano03->Element[0][2] = vec2[0]; matPlano03->Element[0][3] = 0;
	matPlano03->Element[1][0] = vec1[1]; matPlano03->Element[1][1] = vec3[1]; matPlano03->Element[1][2] = vec2[1]; matPlano03->Element[1][3] = 0;
	matPlano03->Element[2][0] = vec1[2]; matPlano03->Element[2][1] = vec3[2]; matPlano03->Element[2][2] = vec2[2]; matPlano03->Element[2][3] = 0;
	matPlano03->Element[3][0] = 0; matPlano03->Element[3][1] = 0; matPlano03->Element[3][2] = 0; matPlano03->Element[3][3] = 1;
	auto newTransform03 = vtkSmartPointer<vtkTransform>::New();
	newTransform03->SetMatrix(matPlano03);
	newTransform03->Update();
	planeActor3->SetUserTransform(newTransform03);
	planeActor3->GetProperty()->LightingOff();

	renderWindow->Render();
}

TelaMPR::TelaMPR(HWND handle, SistemaMPR *sistema, int _id) :id(_id)
{
	this->sistema = sistema;
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
	interactor->SetInteractorStyle(style);
	style->SetOperacao(0, VTKIS_SPIN);
	style->SetOperacao(1, VTKIS_SPIN);
	style->SetOperacao(2, VTKIS_SPIN);
	style->SetSistema(sistema);
	renderWindow->Render();
}

void TelaMPR::Resize(int w, int h)
{
	renderWindow->SetSize(w, h);
}
