#include "Particle.hpp"
#include <cmath>
#include <iostream> // to test
#include <random>

void random_test()
{
  // Create a random number generator
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<double> uniformg(0.0, 1.0);
  std::uniform_real_distribution<double> phig(0.0, 2 * M_PI);
  std::uniform_real_distribution<double> thetag(0.0, M_PI);
  std::exponential_distribution<double> impulseg(1.0);

  int Nbase = 100;
  unsigned int Nmax  = 140;
  int nGen  = 1E5;

  for (int i = 0; i < nGen; ++i) {
    Particle* EventParticle[Nmax];
    int Decay_index = Nbase;

    for (int j = 0; j < Nbase; ++j) {
      EventParticle[j] = new Particle("buffer");

      double phi       = phig(gen);
      double theta     = thetag(gen);
      double impulse   = impulseg(gen);
      double impulse_x = impulse * sin(theta) * cos(phi);
      double impulse_y = impulse * sin(theta) * sin(phi);
      double impulse_z = impulse * cos(theta);
      EventParticle[j]->SetP(impulse_x, impulse_y, impulse_z);

      // Set type with probability
      double chPart = uniformg(gen);
      // double chPart = gRandom->Rndm();
      if (chPart < 0.4) {
        EventParticle[j]->SetIndex("Pi+");
        std::cout << "EventParticle[" << j << "] = " << EventParticle[j]->GetIndex() << std::endl; // to debug
      } else if (chPart >= 0.4 && chPart < 0.8) {
        EventParticle[j]->SetIndex("Pi-");
        std::cout << "EventParticle[" << j << "] = " << EventParticle[j]->GetIndex() << std::endl; // to debug
      } else if (chPart >= 0.8 && chPart < 0.85) {
        EventParticle[j]->SetIndex("K+");
        std::cout << "EventParticle[" << j << "] = " << EventParticle[j]->GetIndex() << std::endl; // to debug
      } else if (chPart >= 0.85 && chPart < 0.9) {
        EventParticle[j]->SetIndex("K-");
        std::cout << "EventParticle[" << j << "] = " << EventParticle[j]->GetIndex() << std::endl; // to debug
      } else if (chPart >= 0.9 && chPart < 0.945) {
        EventParticle[j]->SetIndex("P+");
        std::cout << "EventParticle[" << j << "] = " << EventParticle[j]->GetIndex() << std::endl; // to debug
      } else if (chPart >= 0.945 && chPart < 0.99) {
        EventParticle[j]->SetIndex("P-");
        std::cout << "EventParticle[" << j << "] = " << EventParticle[j]->GetIndex() << std::endl; // to debug
      } else if (chPart >= 0.99) {
        EventParticle[j]->SetIndex("K*");
        std::cout << "EventParticle[" << j << "] = " << EventParticle[j]->GetIndex() << std::endl; // to debug

        double kdecay = uniformg(gen);
        if (kdecay <= 0.5) {
          EventParticle[Decay_index]     = new Particle("K+");
          EventParticle[Decay_index + 1] = new Particle("Pi-");
        } else {
          EventParticle[Decay_index]     = new Particle("K-");
          EventParticle[Decay_index + 1] = new Particle("Pi+");
        }

        // Part 4
        std::cout << "number of particles: " << Particle::GetNParticles() << std::endl;

        EventParticle[j]->Decay2body(*EventParticle[Decay_index], *EventParticle[Decay_index + 1]);
        Decay_index += 2;
      }
    }

    for (int l = 0; i < Particle::GetNParticles(); i++) {
      std::cout << "Array element " << l << ": " << EventParticle[i] << std::endl;
      // if (EventParticle[i] != nullptr) {
      std::cout << "Array element " << l << ": " << EventParticle[i]->GetIndex() << std::endl;
      // }
    } // to debug

    for (int j = 0; j < Particle::GetNParticles(); j++) { //-1 TO SEE
      if (EventParticle[j] != nullptr && EventParticle[j]->GetIndex() != 6) {
        for (int k = j + 1; k < Particle::GetNParticles(); k++) {
          if (EventParticle[k] != nullptr && EventParticle[k]->GetIndex() != 6) {

            // da testare la velocità di esecuzione in confronto all'opzione di esplicitare tutte le combinazioni
            if (((EventParticle[j]->GetIndex() == 0 || EventParticle[j]->GetIndex() == 2
                  || EventParticle[j]->GetIndex() == 4)
                 && (EventParticle[k]->GetIndex() == 0 || EventParticle[k]->GetIndex() == 2
                     || EventParticle[k]->GetIndex() == 4))
                || ((EventParticle[j]->GetIndex() == 1 || EventParticle[j]->GetIndex() == 3
                     || EventParticle[j]->GetIndex() == 5)
                    && (EventParticle[k]->GetIndex() == 1 || EventParticle[k]->GetIndex() == 3
                        || EventParticle[k]->GetIndex() == 5))) {
            }

            if (((EventParticle[j]->GetIndex() == 0 || EventParticle[j]->GetIndex() == 2
                  || EventParticle[j]->GetIndex() == 4)
                 && (EventParticle[k]->GetIndex() == 1 || EventParticle[k]->GetIndex() == 3
                     || EventParticle[k]->GetIndex() == 5))
                || ((EventParticle[j]->GetIndex() == 1 || EventParticle[j]->GetIndex() == 3
                     || EventParticle[j]->GetIndex() == 5)
                    && (EventParticle[k]->GetIndex() == 0 || EventParticle[k]->GetIndex() == 2
                        || EventParticle[k]->GetIndex() == 4))) {
            }

            if ((EventParticle[j]->GetIndex() == 0 && EventParticle[k]->GetIndex() == 2)
                || (EventParticle[j]->GetIndex() == 2 && EventParticle[k]->GetMass() == 0)
                || (EventParticle[j]->GetIndex() == 1 && EventParticle[k]->GetIndex() == 3)
                || (EventParticle[j]->GetIndex() == 3 && EventParticle[k]->GetMass() == 1)) {
            }

            if ((EventParticle[j]->GetIndex() == 0 && EventParticle[k]->GetIndex() == 3)
                || (EventParticle[j]->GetIndex() == 3 && EventParticle[k]->GetMass() == 0)
                || (EventParticle[j]->GetIndex() == 1 && EventParticle[k]->GetIndex() == 2)
                || (EventParticle[j]->GetIndex() == 2 && EventParticle[k]->GetMass() == 1)) {
            }

          } else {
            continue;
          }
        }
      } else {
        continue;
      }
    }
  }
}