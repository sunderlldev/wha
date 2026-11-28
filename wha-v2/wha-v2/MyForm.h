#pragma once

namespace whav2 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	public ref class MyForm : public System::Windows::Forms::Form {
	public:
		MyForm(void);

	protected:
		~MyForm();
		virtual bool ProcessDialogKey(System::Windows::Forms::Keys keyData) override;
		virtual void OnKeyPress(System::Windows::Forms::KeyPressEventArgs^ e) override;
		virtual void WndProc(System::Windows::Forms::Message% m) override;
		virtual void OnPaintBackground(System::Windows::Forms::PaintEventArgs^ e) override;
		virtual void OnPaint(System::Windows::Forms::PaintEventArgs^ e) override;

	private:
		System::ComponentModel::IContainer^ components;
		System::Windows::Forms::Timer^ timerJuego;
		System::Windows::Forms::Panel^ pnlMenu;
		System::Windows::Forms::Button^ btnJugar;
		System::Windows::Forms::Button^ btnControles;
		System::Windows::Forms::Button^ btnLore;
		System::Windows::Forms::Button^ btnCreditos;
		System::Windows::Forms::Button^ btnCargar;
		System::Windows::Forms::Button^ btnSalir;
		System::Windows::Forms::Button^ btnHudControles;
		System::Windows::Forms::Button^ btnHudInventario;
		System::Windows::Forms::Button^ btnHudMisiones;
		System::Windows::Forms::Button^ btnHudPersonajes;
		System::Windows::Forms::Button^ btnHudAyuda;

#pragma region Windows Form Designer generated code
		void InitializeComponent(void) {
			this->components = (gcnew System::ComponentModel::Container());
			this->timerJuego = (gcnew System::Windows::Forms::Timer(this->components));
			this->pnlMenu = (gcnew System::Windows::Forms::Panel());
			this->btnSalir = (gcnew System::Windows::Forms::Button());
			this->btnCargar = (gcnew System::Windows::Forms::Button());
			this->btnCreditos = (gcnew System::Windows::Forms::Button());
			this->btnLore = (gcnew System::Windows::Forms::Button());
			this->btnControles = (gcnew System::Windows::Forms::Button());
			this->btnJugar = (gcnew System::Windows::Forms::Button());
			this->btnHudControles = (gcnew System::Windows::Forms::Button());
			this->btnHudInventario = (gcnew System::Windows::Forms::Button());
			this->btnHudMisiones = (gcnew System::Windows::Forms::Button());
			this->btnHudPersonajes = (gcnew System::Windows::Forms::Button());
			this->btnHudAyuda = (gcnew System::Windows::Forms::Button());
			this->pnlMenu->SuspendLayout();
			this->SuspendLayout();
			// 
			// timerJuego
			// 
			this->timerJuego->Interval = 25;
			this->timerJuego->Tick += gcnew System::EventHandler(this, &MyForm::timerJuego_Tick);
			// 
			// pnlMenu
			// 
			this->pnlMenu->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(15)), static_cast<System::Int32>(static_cast<System::Byte>(24)),
				static_cast<System::Int32>(static_cast<System::Byte>(34)));
			this->pnlMenu->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->pnlMenu->Controls->Add(this->btnSalir);
			this->pnlMenu->Controls->Add(this->btnCargar);
			this->pnlMenu->Controls->Add(this->btnCreditos);
			this->pnlMenu->Controls->Add(this->btnLore);
			this->pnlMenu->Controls->Add(this->btnControles);
			this->pnlMenu->Controls->Add(this->btnJugar);
			this->pnlMenu->Dock = System::Windows::Forms::DockStyle::Fill;
			this->pnlMenu->Location = System::Drawing::Point(0, 0);
			this->pnlMenu->Name = L"pnlMenu";
			this->pnlMenu->Size = System::Drawing::Size(960, 600);
			this->pnlMenu->TabIndex = 0;
			// 
			// btnSalir
			// 
			this->btnSalir->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(234)), static_cast<System::Int32>(static_cast<System::Byte>(224)),
				static_cast<System::Int32>(static_cast<System::Byte>(200)));
			this->btnSalir->Cursor = System::Windows::Forms::Cursors::Hand;
			this->btnSalir->FlatAppearance->BorderSize = 0;
			this->btnSalir->FlatAppearance->MouseDownBackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(210)),
				static_cast<System::Int32>(static_cast<System::Byte>(196)), static_cast<System::Int32>(static_cast<System::Byte>(170)));
			this->btnSalir->FlatAppearance->MouseOverBackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(247)),
				static_cast<System::Int32>(static_cast<System::Byte>(240)), static_cast<System::Int32>(static_cast<System::Byte>(224)));
			this->btnSalir->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btnSalir->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10, System::Drawing::FontStyle::Bold));
			this->btnSalir->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(48)), static_cast<System::Int32>(static_cast<System::Byte>(40)),
				static_cast<System::Int32>(static_cast<System::Byte>(34)));
			this->btnSalir->Location = System::Drawing::Point(730, 520);
			this->btnSalir->Name = L"btnSalir";
			this->btnSalir->Size = System::Drawing::Size(194, 38);
			this->btnSalir->TabIndex = 5;
			this->btnSalir->Text = L"SALIR";
			this->btnSalir->UseVisualStyleBackColor = false;
			this->btnSalir->Click += gcnew System::EventHandler(this, &MyForm::btnSalir_Click);
			// 
			// btnCargar
			// 
			this->btnCargar->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(234)), static_cast<System::Int32>(static_cast<System::Byte>(224)),
				static_cast<System::Int32>(static_cast<System::Byte>(200)));
			this->btnCargar->Cursor = System::Windows::Forms::Cursors::Hand;
			this->btnCargar->FlatAppearance->BorderSize = 0;
			this->btnCargar->FlatAppearance->MouseDownBackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(210)),
				static_cast<System::Int32>(static_cast<System::Byte>(196)), static_cast<System::Int32>(static_cast<System::Byte>(170)));
			this->btnCargar->FlatAppearance->MouseOverBackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(247)),
				static_cast<System::Int32>(static_cast<System::Byte>(240)), static_cast<System::Int32>(static_cast<System::Byte>(224)));
			this->btnCargar->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btnCargar->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10, System::Drawing::FontStyle::Bold));
			this->btnCargar->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(48)), static_cast<System::Int32>(static_cast<System::Byte>(40)),
				static_cast<System::Int32>(static_cast<System::Byte>(34)));
			this->btnCargar->Location = System::Drawing::Point(730, 472);
			this->btnCargar->Name = L"btnCargar";
			this->btnCargar->Size = System::Drawing::Size(194, 38);
			this->btnCargar->TabIndex = 4;
			this->btnCargar->Text = L"CARGAR PARTIDA";
			this->btnCargar->UseVisualStyleBackColor = false;
			this->btnCargar->Click += gcnew System::EventHandler(this, &MyForm::btnCargar_Click);
			// 
			// btnCreditos
			// 
			this->btnCreditos->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(234)), static_cast<System::Int32>(static_cast<System::Byte>(224)),
				static_cast<System::Int32>(static_cast<System::Byte>(200)));
			this->btnCreditos->Cursor = System::Windows::Forms::Cursors::Hand;
			this->btnCreditos->FlatAppearance->BorderSize = 0;
			this->btnCreditos->FlatAppearance->MouseDownBackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(210)),
				static_cast<System::Int32>(static_cast<System::Byte>(196)), static_cast<System::Int32>(static_cast<System::Byte>(170)));
			this->btnCreditos->FlatAppearance->MouseOverBackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(247)),
				static_cast<System::Int32>(static_cast<System::Byte>(240)), static_cast<System::Int32>(static_cast<System::Byte>(224)));
			this->btnCreditos->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btnCreditos->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10, System::Drawing::FontStyle::Bold));
			this->btnCreditos->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(48)), static_cast<System::Int32>(static_cast<System::Byte>(40)),
				static_cast<System::Int32>(static_cast<System::Byte>(34)));
			this->btnCreditos->Location = System::Drawing::Point(730, 424);
			this->btnCreditos->Name = L"btnCreditos";
			this->btnCreditos->Size = System::Drawing::Size(194, 38);
			this->btnCreditos->TabIndex = 3;
			this->btnCreditos->Text = L"CRÉDITOS DEL PROYECTO";
			this->btnCreditos->UseVisualStyleBackColor = false;
			this->btnCreditos->Click += gcnew System::EventHandler(this, &MyForm::btnCreditos_Click);
			// 
			// btnLore
			// 
			this->btnLore->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(234)), static_cast<System::Int32>(static_cast<System::Byte>(224)),
				static_cast<System::Int32>(static_cast<System::Byte>(200)));
			this->btnLore->Cursor = System::Windows::Forms::Cursors::Hand;
			this->btnLore->FlatAppearance->BorderSize = 0;
			this->btnLore->FlatAppearance->MouseDownBackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(210)),
				static_cast<System::Int32>(static_cast<System::Byte>(196)), static_cast<System::Int32>(static_cast<System::Byte>(170)));
			this->btnLore->FlatAppearance->MouseOverBackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(247)),
				static_cast<System::Int32>(static_cast<System::Byte>(240)), static_cast<System::Int32>(static_cast<System::Byte>(224)));
			this->btnLore->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btnLore->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10, System::Drawing::FontStyle::Bold));
			this->btnLore->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(48)), static_cast<System::Int32>(static_cast<System::Byte>(40)),
				static_cast<System::Int32>(static_cast<System::Byte>(34)));
			this->btnLore->Location = System::Drawing::Point(730, 376);
			this->btnLore->Name = L"btnLore";
			this->btnLore->Size = System::Drawing::Size(194, 38);
			this->btnLore->TabIndex = 2;
			this->btnLore->Text = L"LORE DE LA OBRA";
			this->btnLore->UseVisualStyleBackColor = false;
			this->btnLore->Click += gcnew System::EventHandler(this, &MyForm::btnLore_Click);
			// 
			// btnControles
			// 
			this->btnControles->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(234)), static_cast<System::Int32>(static_cast<System::Byte>(224)),
				static_cast<System::Int32>(static_cast<System::Byte>(200)));
			this->btnControles->Cursor = System::Windows::Forms::Cursors::Hand;
			this->btnControles->FlatAppearance->BorderSize = 0;
			this->btnControles->FlatAppearance->MouseDownBackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(210)),
				static_cast<System::Int32>(static_cast<System::Byte>(196)), static_cast<System::Int32>(static_cast<System::Byte>(170)));
			this->btnControles->FlatAppearance->MouseOverBackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(247)),
				static_cast<System::Int32>(static_cast<System::Byte>(240)), static_cast<System::Int32>(static_cast<System::Byte>(224)));
			this->btnControles->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btnControles->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10, System::Drawing::FontStyle::Bold));
			this->btnControles->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(48)), static_cast<System::Int32>(static_cast<System::Byte>(40)),
				static_cast<System::Int32>(static_cast<System::Byte>(34)));
			this->btnControles->Location = System::Drawing::Point(730, 328);
			this->btnControles->Name = L"btnControles";
			this->btnControles->Size = System::Drawing::Size(194, 38);
			this->btnControles->TabIndex = 1;
			this->btnControles->Text = L"CÓMO JUGAR";
			this->btnControles->UseVisualStyleBackColor = false;
			this->btnControles->Click += gcnew System::EventHandler(this, &MyForm::btnControles_Click);
			// 
			// btnJugar
			// 
			this->btnJugar->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(222)), static_cast<System::Int32>(static_cast<System::Byte>(196)),
				static_cast<System::Int32>(static_cast<System::Byte>(140)));
			this->btnJugar->Cursor = System::Windows::Forms::Cursors::Hand;
			this->btnJugar->FlatAppearance->BorderSize = 0;
			this->btnJugar->FlatAppearance->MouseDownBackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(198)),
				static_cast<System::Int32>(static_cast<System::Byte>(172)), static_cast<System::Int32>(static_cast<System::Byte>(120)));
			this->btnJugar->FlatAppearance->MouseOverBackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(238)),
				static_cast<System::Int32>(static_cast<System::Byte>(212)), static_cast<System::Int32>(static_cast<System::Byte>(154)));
			this->btnJugar->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btnJugar->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10, System::Drawing::FontStyle::Bold));
			this->btnJugar->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(48)), static_cast<System::Int32>(static_cast<System::Byte>(40)),
				static_cast<System::Int32>(static_cast<System::Byte>(34)));
			this->btnJugar->Location = System::Drawing::Point(730, 280);
			this->btnJugar->Name = L"btnJugar";
			this->btnJugar->Size = System::Drawing::Size(194, 38);
			this->btnJugar->TabIndex = 0;
			this->btnJugar->Text = L"SELECCIONAR NIVEL";
			this->btnJugar->UseVisualStyleBackColor = false;
			this->btnJugar->Click += gcnew System::EventHandler(this, &MyForm::btnJugar_Click);
			// 
			// btnHudControles
			// 
			this->btnHudControles->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(12)), static_cast<System::Int32>(static_cast<System::Byte>(20)),
				static_cast<System::Int32>(static_cast<System::Byte>(30)));
			this->btnHudControles->Cursor = System::Windows::Forms::Cursors::Hand;
			this->btnHudControles->FlatAppearance->BorderSize = 0;
			this->btnHudControles->FlatAppearance->MouseOverBackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(24)),
				static_cast<System::Int32>(static_cast<System::Byte>(36)), static_cast<System::Int32>(static_cast<System::Byte>(48)));
			this->btnHudControles->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btnHudControles->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.5F, System::Drawing::FontStyle::Bold));
			this->btnHudControles->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(226)), static_cast<System::Int32>(static_cast<System::Byte>(232)),
				static_cast<System::Int32>(static_cast<System::Byte>(240)));
			this->btnHudControles->Location = System::Drawing::Point(760, 16);
			this->btnHudControles->Name = L"btnHudControles";
			this->btnHudControles->Size = System::Drawing::Size(184, 30);
			this->btnHudControles->TabIndex = 1;
			this->btnHudControles->TabStop = false;
			this->btnHudControles->Text = L"VER CONTROLES";
			this->btnHudControles->UseVisualStyleBackColor = false;
			this->btnHudControles->Visible = false;
			this->btnHudControles->Click += gcnew System::EventHandler(this, &MyForm::btnHudControles_Click);
			// 
			// btnHudInventario
			// 
			this->btnHudInventario->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(12)), static_cast<System::Int32>(static_cast<System::Byte>(20)),
				static_cast<System::Int32>(static_cast<System::Byte>(30)));
			this->btnHudInventario->Cursor = System::Windows::Forms::Cursors::Hand;
			this->btnHudInventario->FlatAppearance->BorderSize = 0;
			this->btnHudInventario->FlatAppearance->MouseOverBackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(24)),
				static_cast<System::Int32>(static_cast<System::Byte>(36)), static_cast<System::Int32>(static_cast<System::Byte>(48)));
			this->btnHudInventario->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btnHudInventario->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.5F, System::Drawing::FontStyle::Bold));
			this->btnHudInventario->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(226)),
				static_cast<System::Int32>(static_cast<System::Byte>(232)), static_cast<System::Int32>(static_cast<System::Byte>(240)));
			this->btnHudInventario->Location = System::Drawing::Point(760, 52);
			this->btnHudInventario->Name = L"btnHudInventario";
			this->btnHudInventario->Size = System::Drawing::Size(184, 30);
			this->btnHudInventario->TabIndex = 2;
			this->btnHudInventario->TabStop = false;
			this->btnHudInventario->Text = L"INVENTARIO";
			this->btnHudInventario->UseVisualStyleBackColor = false;
			this->btnHudInventario->Visible = false;
			this->btnHudInventario->Click += gcnew System::EventHandler(this, &MyForm::btnHudInventario_Click);
			// 
			// btnHudMisiones
			// 
			this->btnHudMisiones->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(12)), static_cast<System::Int32>(static_cast<System::Byte>(20)),
				static_cast<System::Int32>(static_cast<System::Byte>(30)));
			this->btnHudMisiones->Cursor = System::Windows::Forms::Cursors::Hand;
			this->btnHudMisiones->FlatAppearance->BorderSize = 0;
			this->btnHudMisiones->FlatAppearance->MouseOverBackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(24)),
				static_cast<System::Int32>(static_cast<System::Byte>(36)), static_cast<System::Int32>(static_cast<System::Byte>(48)));
			this->btnHudMisiones->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btnHudMisiones->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.5F, System::Drawing::FontStyle::Bold));
			this->btnHudMisiones->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(214)), static_cast<System::Int32>(static_cast<System::Byte>(198)),
				static_cast<System::Int32>(static_cast<System::Byte>(166)));
			this->btnHudMisiones->Location = System::Drawing::Point(760, 88);
			this->btnHudMisiones->Name = L"btnHudMisiones";
			this->btnHudMisiones->Size = System::Drawing::Size(184, 30);
			this->btnHudMisiones->TabIndex = 3;
			this->btnHudMisiones->TabStop = false;
			this->btnHudMisiones->Text = L"VER MISIONES [Q]";
			this->btnHudMisiones->UseVisualStyleBackColor = false;
			this->btnHudMisiones->Visible = false;
			this->btnHudMisiones->Click += gcnew System::EventHandler(this, &MyForm::btnHudMisiones_Click);
			// 
			// btnHudPersonajes
			// 
			this->btnHudPersonajes->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(12)), static_cast<System::Int32>(static_cast<System::Byte>(20)),
				static_cast<System::Int32>(static_cast<System::Byte>(30)));
			this->btnHudPersonajes->Cursor = System::Windows::Forms::Cursors::Hand;
			this->btnHudPersonajes->FlatAppearance->BorderSize = 0;
			this->btnHudPersonajes->FlatAppearance->MouseOverBackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(24)),
				static_cast<System::Int32>(static_cast<System::Byte>(36)), static_cast<System::Int32>(static_cast<System::Byte>(48)));
			this->btnHudPersonajes->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btnHudPersonajes->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.5F, System::Drawing::FontStyle::Bold));
			this->btnHudPersonajes->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(214)),
				static_cast<System::Int32>(static_cast<System::Byte>(198)), static_cast<System::Int32>(static_cast<System::Byte>(166)));
			this->btnHudPersonajes->Location = System::Drawing::Point(760, 124);
			this->btnHudPersonajes->Name = L"btnHudPersonajes";
			this->btnHudPersonajes->Size = System::Drawing::Size(184, 30);
			this->btnHudPersonajes->TabIndex = 4;
			this->btnHudPersonajes->TabStop = false;
			this->btnHudPersonajes->Text = L"PERSONAJES [P]";
			this->btnHudPersonajes->UseVisualStyleBackColor = false;
			this->btnHudPersonajes->Visible = false;
			this->btnHudPersonajes->Click += gcnew System::EventHandler(this, &MyForm::btnHudPersonajes_Click);
			// 
			// btnHudAyuda
			// 
			this->btnHudAyuda->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(12)), static_cast<System::Int32>(static_cast<System::Byte>(20)),
				static_cast<System::Int32>(static_cast<System::Byte>(30)));
			this->btnHudAyuda->Cursor = System::Windows::Forms::Cursors::Hand;
			this->btnHudAyuda->FlatAppearance->BorderSize = 0;
			this->btnHudAyuda->FlatAppearance->MouseOverBackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(24)),
				static_cast<System::Int32>(static_cast<System::Byte>(36)), static_cast<System::Int32>(static_cast<System::Byte>(48)));
			this->btnHudAyuda->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btnHudAyuda->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9, System::Drawing::FontStyle::Bold));
			this->btnHudAyuda->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(190)), static_cast<System::Int32>(static_cast<System::Byte>(206)),
				static_cast<System::Int32>(static_cast<System::Byte>(224)));
			this->btnHudAyuda->Location = System::Drawing::Point(760, 160);
			this->btnHudAyuda->Name = L"btnHudAyuda";
			this->btnHudAyuda->Size = System::Drawing::Size(184, 28);
			this->btnHudAyuda->TabIndex = 5;
			this->btnHudAyuda->TabStop = false;
			this->btnHudAyuda->Text = L"QUITAR AYUDA [H]";
			this->btnHudAyuda->UseVisualStyleBackColor = false;
			this->btnHudAyuda->Visible = false;
			this->btnHudAyuda->Click += gcnew System::EventHandler(this, &MyForm::btnHudAyuda_Click);
			// 
			// MyForm
			// 
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::None;
			this->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(7)), static_cast<System::Int32>(static_cast<System::Byte>(11)),
				static_cast<System::Int32>(static_cast<System::Byte>(17)));
			this->ClientSize = System::Drawing::Size(960, 600);
			this->Controls->Add(this->btnHudAyuda);
			this->Controls->Add(this->btnHudPersonajes);
			this->Controls->Add(this->btnHudMisiones);
			this->Controls->Add(this->btnHudInventario);
			this->Controls->Add(this->btnHudControles);
			this->Controls->Add(this->pnlMenu);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->KeyPreview = true;
			this->MaximizeBox = false;
			this->Name = L"MyForm";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"Witch Hat Atelier · El Árbol de Plata";
			this->pnlMenu->ResumeLayout(false);
			this->ResumeLayout(false);

		}
#pragma endregion

		int estadoActual;
		int nivelJugando;
		System::IntPtr ptrNivel1;
		System::IntPtr ptrNivel2;
		System::IntPtr ptrNivel3;
		System::Drawing::Graphics^ gPantalla;
		System::Drawing::BufferedGraphicsContext^ espacioBuffer;
		System::Drawing::BufferedGraphics^ bufferGrafico;
		System::Drawing::Bitmap^ fondoMenu;
		bool bufferListo;

		bool teclaArriba;
		bool teclaAbajo;
		bool teclaIzquierda;
		bool teclaDerecha;
		bool teclaAccion;
		bool teclaShift;
		bool nivel1Completado;
		bool nivel2Completado;
		bool nivel3Completado;

		void CargarProgresoNiveles();
		void GuardarProgresoNiveles();

		void DibujarPino(System::Drawing::Graphics^ g, int x, int baseY, int alto);
		void GenerarFondoMenu();
		System::Drawing::Bitmap^ CargarFondoMenuOriginal();
		void GenerarFondoMenuDesdeImagen();
		void ActualizarTextoBotonJugar();
		void MostrarMenuPrincipal();
		void IniciarNivelSeleccionado(int numNivel, bool reiniciar);
		void ConfigurarPersonajesEnControles(System::Windows::Forms::Form^ frmBase);
		void AbrirControlesRapido();
		void AbrirInventarioModal();
		void AbrirGuardarCargarModal();
		void AbrirPersonajesModal();
		void AbrirInventarioRapido();
		void AbrirPersonajesRapido();
		void AbrirMisionesRapido();
		void AlternarAyudaHud();
		void AbrirPausa();
		void AbrirMinijuego();
		void AbrirTrollface();
		void AbrirFinNivel(int numNivel, bool esDerrota);

		void btnJugar_Click(System::Object^ sender, System::EventArgs^ e);
		void btnControles_Click(System::Object^ sender, System::EventArgs^ e);
		void btnLore_Click(System::Object^ sender, System::EventArgs^ e);
		void btnCreditos_Click(System::Object^ sender, System::EventArgs^ e);
		void btnCargar_Click(System::Object^ sender, System::EventArgs^ e);
		void btnSalir_Click(System::Object^ sender, System::EventArgs^ e);
		void btnHudControles_Click(System::Object^ sender, System::EventArgs^ e);
		void btnHudInventario_Click(System::Object^ sender, System::EventArgs^ e);
		void btnHudMisiones_Click(System::Object^ sender, System::EventArgs^ e);
		void btnHudPersonajes_Click(System::Object^ sender, System::EventArgs^ e);
		void btnHudAyuda_Click(System::Object^ sender, System::EventArgs^ e);
		void timerJuego_Tick(System::Object^ sender, System::EventArgs^ e);
		void MyForm_PreviewKeyDown(System::Object^ sender, System::Windows::Forms::PreviewKeyDownEventArgs^ e);
		void MyForm_KeyDown(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e);
		void MyForm_KeyUp(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e);
		void MyForm_Deactivate(System::Object^ sender, System::EventArgs^ e);
};
}
