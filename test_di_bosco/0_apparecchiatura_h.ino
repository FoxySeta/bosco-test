// 0_apparecchiatura_h: intestazione del modulo per la gestione della barriera laser

#ifndef APPARECCHIATURA_H
#define APPARECCHIATURA_H

namespace Apparecchiatura {

	// una barriera dovrebbe avere l'esclusiva sull'accesso ai propri pin
	class Barriera {
	public:
		// inizializzazione obbligatoria
		Barriera(int pin_laser_a, int pin_laser_b, int pin_foto_a, int pin_foto_b); // [a, b[
		Barriera() = delete;
		// spostamento consentito
		Barriera(Barriera&&) = default;
		Barriera& operator=(Barriera&&) = default;
		// copia vietata: un pin dovrebbe appartenere a una sola barriera
		Barriera(const Barriera&) = default;
		Barriera& operator=(const Barriera&) = delete;
		// la barriera viene spenta alla distruzione
		~Barriera();

		// costanti e getter
		static constexpr int tolleranza { 25 }; // fra lettura luce naturale e lettura a barriera accesa con ostacolo
		int pin_laser_a() const;
		int pin_laser_b() const;
		int pin_foto_a() const;
		int pin_foto_b() const;
		bool stato() const; // true = accesa, false = spenta

		// false: la barriera era già nello stato desiderato
		bool accendi();
		bool spegni();
		bool ostacolo() const; // false se non ci sono ostacoli o a barriera spenta

	private:
		int pin_l_a, pin_l_b, pin_f_a, pin_f_b; // primi e ultimi pin per laser e fotoresistenze
		bool st; // true = accesa, false = spenta
		int luce_naturale { 0 };
		
		void imposta_laser(int valore); // HIGH o LOW
		int leggi_fotoresistenze() const; // media delle analogRead()
	};
	
}

#endif
