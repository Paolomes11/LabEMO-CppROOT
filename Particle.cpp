#include "Particle.hpp"
#include <iostream>
#include <cstring>  

ParticleType* Particle::fParticleType_[fMaxNumParticleType] = {nullptr};
int Particle::fNParticleType = 0;  

Particle::Particle(const char* fName, double fPx, double fPy, double fPz) 
    : fPx_(fPx), fPy_(fPy), fPz_(fPz) {
    
    fIndex_ = FindParticle(fName);

    if (fIndex_ == -1) {
        std::cerr << "Errore: tipo di particella '" << fName << "' non trovato!" << std::endl;
    }
}

int Particle::FindParticle(const char* fName) {
    for (int i = 0; i < fMaxNumParticleType; ++i) {
        if (fParticleType_[i] != nullptr && strcmp(fParticleType_[i]->GetName(), fName) == 0) {
            return i;  
        }
    }
    return -1;  
}

void Particle::AddParticleType(const char* nome, double massa, int carica, double larghezza) {
    if (FindParticle(nome) != -1) {
        std::cerr << "Errore: tipo di particella '" << nome << "' già presente!" << std::endl;  
        return;  
    }

    if (fNParticleType >= fMaxNumParticleType) {
        std::cerr << "Errore: numero massimo di tipi di particella raggiunto!" << std::endl; 
        return;  
    }

    ParticleType* newParticleType;
    if (larghezza > 0) {
        newParticleType = new ResonanceType(nome, massa, carica, larghezza);
    } else {
        newParticleType = new ParticleType(nome, massa, carica);
    }

    for (int i = 0; i < fMaxNumParticleType; ++i) {
        if (fParticleType_[i] == nullptr) {  
            fParticleType_[i] = newParticleType;  
            fNParticleType++;  
            return;  
        }
    }
}
