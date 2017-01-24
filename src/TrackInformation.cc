#include "TrackInformation.hh"
#include "G4ios.hh"
#include <iostream>
#include <fstream>

G4Allocator<TrackInformation> aTrackInformationAllocator;

TrackInformation::TrackInformation()
{
   G4cout<<"Pre TrackInfo enter>0 "<<G4endl;
    originalTrackID = 0;
    particleDefinition = 0;
    originalPosition = G4ThreeVector(0.,0.,0.);
    originalMomentum = G4ThreeVector(0.,0.,0.);
    originalEnergy = 0.;
    originalTime = 0.;
    trackingStatus = 1;
    sourceTrackID = -1;
    sourceTrackID = -1;
    sourceDefinition = 0;
    sourcePosition = G4ThreeVector(0.,0.,0.);
    sourceMomentum = G4ThreeVector(0.,0.,0.);
    sourceEnergy = 0.;
    sourceTime = 0.;
    CountCerenkov=0;
    oldCountCerenkov=0;
    PrimaryPhotonID=0;
    particle_ances.push_back(0);
    G4cout<<"Pre TrackInfo exit>0 "<<G4endl;

    //BS
    OrigNstb=0; //nstb=0 indicates particle did not hit FMS. ancestor/decendent may have though. So long as i don't overwrite this in the other constructors, every track should have these unless it's specifically overwritten
    OrigRow=0;
    OrigCol=0;
    HasCoordinates=0;


}

TrackInformation::TrackInformation(const G4Track* aTrack)
{
    originalTrackID = aTrack->GetTrackID();  //Parent particle ID
    particleDefinition = aTrack->GetDefinition();
    originalPosition = aTrack->GetPosition();
    originalMomentum = aTrack->GetMomentum();
    originalEnergy = aTrack->GetTotalEnergy();
    originalTime = aTrack->GetGlobalTime();
    trackingStatus = 1;
    sourceTrackID = -1;
    sourceDefinition = 0;
    sourcePosition = G4ThreeVector(0.,0.,0.);
    sourceMomentum = G4ThreeVector(0.,0.,0.);
    sourceEnergy = 0.;
    sourceTime = 0.;    
    CountCerenkov=0;
    oldCountCerenkov=0;
    PrimaryPhotonID=0;
    particle_ances.push_back(aTrack->GetTrackID());
    OrigNstb=0; //nstb=0 indicates particle did not hit FMS. ancestor/decendent may have though. So long as i don't overwrite this in the other constructors, every track should have these unless it's specifically overwritten
    OrigRow=0;
    OrigCol=0;
    HasCoordinates=0;


}

TrackInformation::TrackInformation(const TrackInformation* aTrackInfo)
{
    originalTrackID = aTrackInfo->originalTrackID;
    particleDefinition = aTrackInfo->particleDefinition;
    originalPosition = aTrackInfo->originalPosition;
    originalMomentum = aTrackInfo->originalMomentum;
    originalEnergy = aTrackInfo->originalEnergy;
    originalTime = aTrackInfo->originalTime;
    trackingStatus = aTrackInfo->trackingStatus;
    sourceTrackID = aTrackInfo->sourceTrackID;
    sourceDefinition = aTrackInfo->sourceDefinition;
    sourcePosition = aTrackInfo->sourcePosition;
    sourceMomentum = aTrackInfo->sourceMomentum;
    sourceEnergy = aTrackInfo->sourceEnergy;
    sourceTime = aTrackInfo->sourceTime;
    CountCerenkov=aTrackInfo->CountCerenkov;
    oldCountCerenkov=aTrackInfo->oldCountCerenkov;
    PrimaryPhotonID=aTrackInfo->PrimaryPhotonID;
    particle_ances=(aTrackInfo->particle_ances);

    OrigNstb=0; //nstb=0 indicates particle did not hit FMS. ancestor/decendent may have though. So long as i don't overwrite this in the other constructors, every track should have these unless it's specifically overwritten
    OrigRow=0;
    OrigCol=0;
    HasCoordinates=0;
    


}

TrackInformation::~TrackInformation()
{ ; }

TrackInformation& TrackInformation::operator =(const TrackInformation& aTrackInfo)
{
    originalTrackID = aTrackInfo.originalTrackID;
    particleDefinition = aTrackInfo.particleDefinition;
    originalPosition = aTrackInfo.originalPosition;
    originalMomentum = aTrackInfo.originalMomentum;
    originalEnergy = aTrackInfo.originalEnergy;
    originalTime = aTrackInfo.originalTime;
    trackingStatus = aTrackInfo.trackingStatus;
    sourceTrackID = aTrackInfo.sourceTrackID;
    sourceDefinition = aTrackInfo.sourceDefinition;
    sourcePosition = aTrackInfo.sourcePosition;
    sourceMomentum = aTrackInfo.sourceMomentum;
    sourceEnergy = aTrackInfo.sourceEnergy;
    sourceTime = aTrackInfo.sourceTime;
    CountCerenkov=aTrackInfo.CountCerenkov;
    oldCountCerenkov=aTrackInfo.oldCountCerenkov;
    PrimaryPhotonID=aTrackInfo.PrimaryPhotonID;
    particle_ances=(aTrackInfo.particle_ances);
    OrigNstb=0; //nstb=0 indicates particle did not hit FMS. ancestor/decendent may have though. So long as i don't overwrite this in the other constructors, every track should have these unless it's specifically overwritten
    OrigRow=0;
    OrigCol=0;
    HasCoordinates=0;
    return *this;
}

void TrackInformation::SetSourceTrackInformation(const G4Track* aTrack)
{
    sourceTrackID = aTrack->GetTrackID();
    sourceDefinition = aTrack->GetDefinition();
    sourcePosition = aTrack->GetPosition();
    sourceMomentum = aTrack->GetMomentum();
    sourceEnergy = aTrack->GetTotalEnergy();
    sourceTime = aTrack->GetGlobalTime();
}

void TrackInformation::Print() const
{
  //printf("sourceDefinition @ %p\n",(void*) sourceDefinition);
  if(!strcmp(sourceDefinition->GetParticleName(),"pi0") || 
     !strcmp(sourceDefinition->GetParticleName(),"gamma") ||
     !strcmp(sourceDefinition->GetParticleName(),"eta") ||
     !strcmp(sourceDefinition->GetParticleName(),"e-") ||
     !strcmp(sourceDefinition->GetParticleName(),"e+"))
  {
    if(sourceEnergy/GeV>0.5 && sourcePosition.getZ()/cm<-30)
    {
      G4DecayTable * decay = (G4DecayTable*) sourceDefinition->GetDecayTable();
      if(decay)
      {
        printf("******* decay @ %p\n",(void*) decay);
        decay->DumpInfo();
        G4VDecayChannel * channel = decay->GetDecayChannel(0);
        G4ParticleDefinition * daught = channel->GetDaughter(0);
        G4cout << "daughter 0: " << daught->GetParticleName() << G4endl;
      }
      
      G4double px = sourceMomentum.getX()/MeV;
      G4double py = sourceMomentum.getY()/MeV;
      G4double pz = sourceMomentum.getZ()/MeV;
      G4double pnorm = sqrt(pow(px,2)+pow(py,2)+pow(pz,2));
      G4ThreeVector normMomentum = sourceMomentum/pnorm;

      G4cout << ":::::::: Type = " << sourceDefinition->GetParticleName() << G4endl;
      G4cout << ":      : Energy = " << sourceEnergy/GeV << "GeV" << G4endl;
      G4cout << ":  ::  : Position = (" <<
        sourcePosition.getX()/cm << ", " <<
        sourcePosition.getY()/cm << ", " <<
        sourcePosition.getZ()/cm << ")" << G4endl;
      G4cout << ":      : Momentum = (" <<
        sourceMomentum.getX()/MeV << ", " <<
        sourceMomentum.getY()/MeV << ", " <<
        sourceMomentum.getZ()/MeV << ") MeV " << G4endl;
      G4cout << ":::::::: Normalised Momentum = (" <<
        normMomentum.getX() << ", " <<
        normMomentum.getY() << ", " <<
        normMomentum.getZ() << ")" << G4endl;

      G4double xmap = 668.9*98.6*px/(98.77*pz);
      G4double ymap = 668.9*98.6*py/(98.77*pz);
      G4int incident=0;
      static G4int evnum=-1;
      if(!strcmp(sourceDefinition->GetParticleName(),"pi0"))
      {
        incident=1;
        evnum++;
      };

    }

    //G4cout << "Source track ID = " << sourceTrackID << G4endl;
    //G4cout << "Particle Name = " << sourceDefinition->GetParticleName() << G4endl;
    //G4cout << "Particle Energy = " << sourceEnergy/GeV << "[GeV]" << G4endl;
    //G4cout << "Original Primary Track ID = " << originalTrackID << G4endl;
    //G4cout << "Original Particle Name = " << particleDefinition->GetParticleName() << G4endl;
    //G4cout << "Original Particle Energy = " << originalEnergy/GeV << "[GeV]" << G4endl;
    //printf("Original Momentum (%f, %f, %f) [MeV]\n",originalMomentum.getX()/MeV,originalMomentum.getY()/MeV,originalMomentum.getZ()/MeV);
   }
}

