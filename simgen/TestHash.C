// checks the pdg kf hash algorithm for accuracy
// must be 100% accurate to be useful

void TestHash()
{
  gROOT->LoadMacro("LoadLibs.C");
  if(!LoadLibs()) { exit(); };
  TString SIMGEN = gSystem->ExpandPathName("${SIMGEN}");
  
  char codesfile[128];
  sprintf(codesfile,"%s/dat/codes.dat",(char*) SIMGEN);
  printf("codesfile=%s\n",codesfile);
 
  HashKF * hash = new HashKF(codesfile);

  hash->CheckHashTable();
  
  return;
};
