/***************************************
*
* generateInvertedFile 
*
* creates an inverted file at the using the 
* files 
*
****************************************
*
* modification history:
*
* WM 991213 created the file
*
****************************************
*
* compiler defines used:
*
* QUERY
*
****************************************/

#include "mrml_const.h"
#include "CXMLElement.h"
#include "CAcIFFileSystem.h"

/***************************************
*
* main - main program, that reads the parameters and 
*        executes the creation of the inverted file 
*
****************************************
*
* modification history
*
* 
*
****************************************/

CMutex* gMutex(new CMutex());

main(int argc,
     char** argv){

  if(argc==2){
    /* reading the parameters */
    string lPrefix(argv[1]);
    string lURL2FTS(lPrefix+"/url2fts.xml");
    string lFeatureDescription(lPrefix+"/InvertedFileFeatureDescription.db");

    cout << "Creating an inverted file using the files"
	 << lPrefix << endl
	 << lURL2FTS << endl
	 << lFeatureDescription << endl
	 << endl
	 << flush;
    
    CXMLElement lElement(mrml_const::collection,
			 0);

    lElement.addAttribute(mrml_const::cui_base_dir,
			  lPrefix);			 
    lElement.addAttribute(mrml_const::cui_feature_file_location,
			  "url2fts.xml");
    lElement.addAttribute(mrml_const::cui_feature_description_location,
			  "InvertedFileFeatureDescription.db");
    lElement.addAttribute(mrml_const::cui_inverted_file_location,
			  "InvertedFile.db");
    lElement.addAttribute(mrml_const::cui_offset_file_location,
			  "InvertedFileOffset.db");
    lElement.addAttribute(mrml_const::cui_generate_inverted_file,
			  "yes");

    CAcIFFileSystem lAccessor(lElement);
    
    if(lAccessor){
      cout << "GENERATED inverted file" 
	   << endl
	   << flush;
      
    }else{
      cout << "ERROR in generating inverted file";
    }
  }else{
    
    cout << "USAGE: "
	 << argv[0] 
	 << " " 
	 << "[inverted file directory]";


  }
}




