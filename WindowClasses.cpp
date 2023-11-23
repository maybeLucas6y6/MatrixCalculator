#include "WindowClasses.h"

std::list<MatriceRezultat> rezultate;
Matrice A("A", "Matrice A"), B("B", "Matrice B");
Operatii _A(&A, &B, "Operatii A");
Operatii _B(&B, &A, "Operatii B");
Eroare eroare;

void   NewWindow::WindowCode() {

}
       NewWindow::NewWindow(const char* _windowTitle) {
	windowTitle = _windowTitle;
}
void   NewWindow::Run() {
	ImGui::Begin(windowTitle, NULL, ImGuiWindowFlags_AlwaysAutoResize);
	// eventual de adaugat parametrii

	WindowCode();

	ImGui::End();
}

     Matrice::Matrice(std::string _nume, const char* _windowTitle)
	: NewWindow(_windowTitle)
{
	values.resize(1, std::vector<float>(1, 0.0f));
	nume = _nume;
}
void Matrice::WindowCode() {
	using namespace ImGui;
	bool changed = false;
	PushItemWidth(70);
	if (InputInt("linii", &rows) || InputInt("coloane", &cols)) changed = true;
	if (rows > 5) rows = 5, changed = false;
	if (rows < 1) rows = 1, changed = false;
	if (cols > 5) cols = 5, changed = false;
	if (cols < 1) cols = 1, changed = false;
	if (changed) {
		values.resize(rows);
		for (std::vector<float>& x : values) {
			x.resize(cols, 0);
		}
	}
	static int inputWidth = 40;
	PushItemWidth(inputWidth);
	// resize not working properly 
	for (int n = 0; n < rows * cols; n++)
	{
		PushID(n);
		InputFloat(" ", &values[n / cols][n % cols], 0, 0, "%.2f", ImGuiInputTextFlags_AutoSelectAll);
		if ((n + 1) % cols != 0) {
			SameLine();
		}
		PopID();
	}
}

     Operatii::Operatii(Matrice* X, Matrice* Y, const char* _windowTitle)
	: NewWindow(_windowTitle) {
	thisMatrix = &(X->values);
	otherMatrix = &(Y->values);
	buttonTexts.push_back(std::string(X->nume + " x "));
	buttonTexts.push_back(std::string(X->nume + " + " + Y->nume));
	buttonTexts.push_back(std::string(X->nume + " - " + Y->nume));
	buttonTexts.push_back(std::string(X->nume + " x " + Y->nume));
	buttonTexts.push_back(std::string(X->nume + " ^ "));
	buttonTexts.push_back(std::string("Transpusa"));
}
void Operatii::InmultireConstanta() {
	using namespace ImGui;

	PushItemWidth(60);
	if (Button(buttonTexts[0].c_str())) {
		Matrix matrix = *thisMatrix;
		for (int i = 0; i < matrix.size(); i++) {
			for (int j = 0; j < matrix[0].size(); j++) {
				matrix[i][j] *= constanta;
			}
		}
		rezultate.push_back(MatriceRezultat(matrix));
	}
	SameLine();
	PushID(97);
	InputFloat(" ", &constanta, 0, 0, "%.2f", ImGuiInputTextFlags_AutoSelectAll);
	PopID();
}
void Operatii::Adunare() {
	using namespace ImGui;

	if (Button(buttonTexts[1].c_str())) {
		Matrix A = *thisMatrix, B = *otherMatrix;
		if (A.size() != B.size() || A[0].size() != B[0].size()) {
			eroare.UpdateMessage("Dimensiunile matricilor trebuie sa fie identice");
			return;
		}

		Matrix C;
		C.resize(A.size(), std::vector<float>(A[0].size()));
		for (int i = 0; i < C.size(); i++) {
			for (int j = 0; j < C[0].size(); j++) {
				C[i][j] = A[i][j] + B[i][j];
			}
		}
		rezultate.push_back(MatriceRezultat(C));
	}
}
void Operatii::Scadere() {
	using namespace ImGui;

	if (Button(buttonTexts[2].c_str())) {
		Matrix A = *thisMatrix, B = *otherMatrix;
		if (A.size() != B.size() || A[0].size() != B[0].size()) {
			eroare.UpdateMessage("Dimensiunile matricilor trebuie sa fie identice");
			return;
		}

		Matrix C;
		C.resize(A.size(), std::vector<float>(A[0].size()));
		for (int i = 0; i < C.size(); i++) {
			for (int j = 0; j < C[0].size(); j++) {
				C[i][j] = A[i][j] - B[i][j];
			}
		}
		rezultate.push_back(MatriceRezultat(C));
	}
}
void Operatii::InmultireMatrice() {

	using namespace ImGui;
	if (Button(buttonTexts[3].c_str())) {
		Matrix A = *thisMatrix, B = *otherMatrix;
		if (A[0].size() != B.size()) {
			eroare.UpdateMessage("Numarul de coloane din prima matrice trebuie sa\nfie egal cu numarul de linii din a doua matrice");
			return;
		}

		Matrix C;
		C.resize(A.size(), std::vector<float>(B[0].size()));
		for (int i = 0; i < A.size(); i++) {
			for (int j = 0; j < B[0].size(); j++) {
				int s = 0;
				for (int z = 0; z < B.size(); z++) {
					s += A[i][z] * B[z][j];
				}
				C[i][j] = s;
			}
		}
		rezultate.push_back(MatriceRezultat(C));
	}
}
void Operatii::Putere() {
	using namespace ImGui;
	PushItemWidth(60);
	if (Button(buttonTexts[4].c_str())) {
		if (putere <= 0) {
			putere = 1;
			return;
		}
		Matrix matrix = *thisMatrix;
		if (matrix[0].size() != matrix.size()) {
			eroare.UpdateMessage("Matricea trebuie sa fie patratica");
			return;
		}
		Matrix C, D;
		D.resize(matrix.size(), std::vector<float>(matrix[0].size()));
		C = matrix;
		while (putere != 1) {
			for (int i = 0; i < matrix.size(); i++) {
				for (int j = 0; j < matrix[0].size(); j++) {
					int s = 0;
					for (int z = 0; z < C.size(); z++) {
						s += matrix[i][z] * C[z][j];
					}
					D[i][j] = s;
				}
			}
			C = D;
			putere--;
		}
		rezultate.push_back(MatriceRezultat(C));
	}
	SameLine();
	PushID(98);
	InputInt(" ", &putere, 0, 0, ImGuiInputTextFlags_AutoSelectAll);
	PopID();
}
void Operatii::Transpusa() {
	using namespace ImGui;
	if (Button(buttonTexts[5].c_str())) {
		Matrix A = *thisMatrix, C;
		C.resize(A[0].size(), std::vector<float>(A.size()));
		for (int i = 0; i < A.size(); i++) {
			for (int j = 0; j < A[0].size(); j++) {
				C[j][i] = A[i][j];
			}
		}
		rezultate.push_back(MatriceRezultat(C));
	}
}
void Operatii::WindowCode() {
	using namespace ImGui;

	InmultireConstanta();
	Adunare();
	Scadere();
	InmultireMatrice();
	Putere();
	Transpusa();
}

int  MatriceRezultat::contor = 0;
     MatriceRezultat::MatriceRezultat(Matrix X) {
	matrix = X;
	contor++;
	title = "Rezultat " + std::to_string(contor);
}
void MatriceRezultat::CreateWindow() {
	using namespace ImGui;

	ImGui::Begin(title.c_str(), &running, ImGuiWindowFlags_AlwaysAutoResize);

	int inputWidth = 40;
	PushItemWidth(inputWidth);
	// resize not working properly 
	int cols = matrix[0].size();
	for (int n = 0; n < matrix.size() * cols; n++)
	{
		PushID(n);
		InputFloat(" ", &matrix[n / cols][n % cols], 0, 0, "%.2f", ImGuiInputTextFlags_ReadOnly);
		if ((n + 1) % cols != 0) {
			SameLine();
		}
		PopID();
	}
	if (Button("Copiaza in A")) {
		A.values = matrix;
		A.rows = matrix.size();
		A.cols = matrix[0].size();
		running = false;
	}
	if (Button("Copiaza in B")) {
		B.values = matrix;
		B.rows = matrix.size();
		B.cols = matrix[0].size();
		running = false;
	}

	ImGui::End();
}

void Eroare::UpdateMessage(std::string _message) {
	message = _message;
	running = true;
}
void Eroare::CreateWindow() {
	using namespace ImGui;
	if (!running) {
		return;
	}
	else {
		OpenPopup("Eroare");
	}

	SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f),
		ImGuiCond_Always, ImVec2(0.5f, 0.5f));
	if (ImGui::BeginPopupModal("Eroare", &running,
		ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove)) {

		Text((const char*)message.c_str());
		ImGui::EndPopup();
	}
}