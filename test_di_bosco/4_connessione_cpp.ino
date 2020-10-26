// 4_connessione_h: implementazione del modulo di connessione bluetooth al terminale

namespace Connessione {
	
	Terminale::Terminale(Apparecchiatura::Barriera& barriera, int pin_tra, int pin_ric,
						 long baud, HardwareSerial& debug, long baud_debug)
		:bar{barriera},
		pin_tx{pin_tra},
		pin_rx{pin_ric},
		m{SoftwareSerial(pin_tx, pin_rx)},
		b{baud}, d{debug},
		b_d{baud_debug} {
		// preparazione modulo di connessione
		m.begin(b);
	}

	Terminale::Terminale(Apparecchiatura::Barriera& barriera, int pin_tra, int pin_ric,
						 HardwareSerial& debug, long baud_debug)
		:Terminale{barriera, pin_tra, pin_ric, baud_def, debug, baud_debug} { }

	Apparecchiatura::Barriera& Terminale::barriera() const {
		return bar;
	}

	int Terminale::pin_tra() const {
		return pin_tx;
	}

	int Terminale::pin_ric() const {
		return pin_rx;
	}

	long Terminale::baud() const {
		return b;
	}

	HardwareSerial& Terminale::debug() const {
		return d;
	}

	long Terminale::baud_debug() const {
		return b_d;
	}

	void Terminale::setup() {
		d.begin(b_d);
		d.println("Modulo di connessione al terminale in funzione.");
	}
	
	void Terminale::loop() {
		if (m.available()) {
			char input { m.read() };
			switch (input) {
			case comando_stato:
				azione_stato();
				break;
			case comando_spegni:
				azione_spegni();
				break;
			case comando_accendi:
				azione_accendi();
				break;
			case comando_chiedi:
				azione_chiedi();
				break;
			case comando_abortisci:
				d.println("Aborto fallito (nessuna richiesta da abortire.");
				break;
			default:
				azione_sconosciuta(input);
			}
		}
	}
	
	void Terminale::azione_stato() {
		m.println(bar.stato());
		d.println("Stato della barriera comunicato.");
	}
	
	void Terminale::azione_spegni() {
		if (bar.spegni())
			d.println("Barriera spenta.");
		else
			d.println("Barriera spenta da prima.");
	}
	
	void Terminale::azione_accendi() {
		if (bar.accendi())
			d.println("Barriera accesa.");
		else
			d.println("Barriera accesa da prima.");		
	}

	void Terminale::azione_chiedi() {
		if (!bar.ostacolo()) {
			m.println(false);
			m.readString(); // ignoro il resto dell'input
			d.println("Atleta non rilevato sulla pedana.");
			return;
		}
		const int salti_richiesti = calcola_salti_richiesti();
		d.print("Salti richiesti: ");
		d.print(salti_richiesti);
		d.println(".");
		for (int i { 0 }; i < salti_richiesti; ++i) {
			while (bar.ostacolo()) // prima dello stacco
				if (azione_aborto())
					return;
			const unsigned long millis_stacco { millis() };
			while (!bar.ostacolo()) // fase di volo
				if (azione_aborto())
					return;
			m.println(millis() - millis_stacco);
			d.print("Salto #");
			d.print(i + 1);
			d.println(" acquisito.");
		}
	}

	bool Terminale::azione_aborto() {
		if (m.available() && m.read() == comando_abortisci) {
			d.println("Prova abortita.");
			return true;
		}
		return false;
	}
	
	void Terminale::azione_sconosciuta(char input) {
		d.print("Comando '");
		d.print(input);
		d.println("' non riconosciuto.");
	}

	int Terminale::calcola_salti_richiesti() {
		if (!m.available())
			return salti_richiesti_default;
		char salti_richiesti[salti_richiesti_string_dim];
		m.readString().toCharArray(salti_richiesti, salti_richiesti_string_dim);
		return atoi(salti_richiesti);
	}
	
}
