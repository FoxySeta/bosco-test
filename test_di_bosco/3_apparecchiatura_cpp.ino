// 3_apparecchiatura_cpp: implementazione del modulo per la gestione della barriera laser

namespace Apparecchiatura {
	
	Barriera::Barriera(int pin_laser_a, int pin_laser_b, int pin_foto_a, int pin_foto_b)
		:pin_l_a{pin_laser_a}, pin_f_a{pin_foto_a} {
		// eventuale correzione dell'input
		pin_l_b = pin_laser_b <= pin_laser_a ? pin_l_a + 1 : pin_laser_b;
		pin_f_b = pin_foto_b <= pin_foto_a ? pin_f_a + 1: pin_foto_b;
		// configurazione pin
		for (int i { pin_l_a }; i < pin_l_b; ++i)
			pinMode(i, OUTPUT);
		// spengimento eventuali laser rimasti accesi da precedente utilizzo
		imposta_laser(LOW);
	}

	Barriera::~Barriera() {
		spegni();
	}

	int Barriera::pin_laser_a() const {
		return pin_l_a;
	}

	int Barriera::pin_laser_b() const {
		return pin_l_b;
	}

	int Barriera::pin_foto_a() const {
		return pin_f_a;
	}

	int Barriera::pin_foto_b() const {
		return pin_f_b;
	}
	
	bool Barriera::stato() const {
		return st;
	}

	bool Barriera::accendi() {
		if (st) // barriera già accesa
			return false;
		luce_naturale = leggi_fotoresistenze();
		imposta_laser(HIGH);
		st = true;
		return true;
	}

	bool Barriera::spegni() {
		if (!(st && luce_naturale)) // barriera già spenta, barriera accesa non correttamente
			return false;
		imposta_laser(LOW);
		st = false;
		return true;
	}

	bool Barriera::ostacolo() const {
		if (!st) // barriera spenta
			return false;
		for (int i { pin_f_a }; i < pin_f_b; ++i)
			if (analogRead(i) <= luce_naturale + tolleranza)
				return true;
		return false;
	}
	
	void Barriera::imposta_laser(int valore) {
		for (int i { pin_l_a }; i < pin_l_b; ++i)
			digitalWrite(i, valore);
	}

	int Barriera::leggi_fotoresistenze() const {
		int somma { 0 };
		for (int i { pin_f_a }; i < pin_f_b; ++i)
			somma += analogRead(i);
		return somma / (pin_f_b - pin_f_a);
	}
}
