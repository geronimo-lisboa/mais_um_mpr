#pragma once
#include "stdafx.h"
class vtkCellPicker;

class SistemaMPR;

class myResliceInteractionStyle : public vtkInteractorStyle
{
public:
	static myResliceInteractionStyle *New();
	vtkTypeMacro(myResliceInteractionStyle, vtkInteractorStyle);
	void PrintSelf(ostream& os, vtkIndent indent) override;

	//@{
	/**
	* Event bindings controlling the effects of pressing mouse buttons
	* or moving the mouse.
	*/
	void OnMouseMove() override;
	void OnLeftButtonDown() override;
	void OnLeftButtonUp() override;
	void OnMiddleButtonDown() override;
	void OnMiddleButtonUp() override;
	void OnRightButtonDown() override;
	void OnRightButtonUp() override;
	//@}

	// These methods for the different interactions in different modes
	// are overridden in subclasses to perform the correct motion. Since
	// they might be called from OnTimer, they do not have mouse coord parameters
	// (use interactor's GetEventPosition and GetLastEventPosition)
	void Rotate() override;
	void Spin() override;
	void Pan() override;
	void Dolly() override;
	void UniformScale() override;

	void SetOperacao(int idBotao, int operacao);

	void SetPropsDoReslice(vtkProp3D *p1, vtkProp3D *p2);
	void SetSistema(SistemaMPR* sist) {
		sistema = sist;
	}
protected:
	SistemaMPR *sistema;
	vtkProp3D *propDoReslice1, *propDoReslice2;

	myResliceInteractionStyle();
	~myResliceInteractionStyle() override;

	void FindPickedActor(int x, int y);

	void Prop3DTransform(vtkProp3D *prop3D,
		double *boxCenter,
		int NumRotation,
		double **rotate,
		double *scale);

	double MotionFactor;
	int currentMouseButton;
	vtkProp3D *InteractionProp;
	vtkCellPicker *InteractionPicker;
	std::array<int, 3> OperacoesDoMouse;
	int isMousePressed;
private:
	myResliceInteractionStyle(const myResliceInteractionStyle&) = delete;
	void operator=(const myResliceInteractionStyle&) = delete;
};

