// loads library for simgen code (src/SimGen.so)

Bool_t LoadLibs()
{
  TString checkstr = gSystem->ExpandPathName("$SIMGENENV");
  if(checkstr!="SIMGENENV")
  {
    fprintf(stderr,"Error: setup SimGen environment first\n");
    return false;
  }

  TString SIMGENDIR = gSystem->ExpandPathName("$SIMGEN");
  TString SIMGENLIB = gSystem->ExpandPathName("$SIMGENLIB");
  char libfile[64];
  sprintf(libfile,"%s/src/%s",(char*) SIMGENDIR,(char*) SIMGENLIB);
  printf("loading %s\n",libfile);

  gSystem->Load(libfile);
  return true;
}
