#include "libGIFTAcInvertedFile/include/CAFInvertedFile.h"
#include "libGIFTAcInvertedFile/include/CAcIFMySQL.h"
#include "libGIFTAcInvertedFile/include/CAcIFFileSystem.h"
#include "libMRML/include/CXMLElement.h"
/**
 */
CAccessor* CAFInvertedFile::makeAccessor(const CXMLElement& inConfiguration)const{

#ifdef __GIFT_WITH_MYSQL__
  pair<bool,string> lInvertedFileType=inConfiguration.stringReadAttribute("cui-inverted-file-type");

  if(lInvertedFileType.first){
    cout << "yes/noMAKING MYSQL" << lInvertedFileType.second << endl;
  }else{
    cout << "not there MAKING MYSQL" << lInvertedFileType.first << endl;

  }

  if((lInvertedFileType.first)
     && (lInvertedFileType.second=="mysql")){

    cout << endl << "���MAKING MYSQL" << endl;

    return new CAcIFMySQL(inConfiguration);
  }else{
    cout << endl << "���NOT MAKING MYSQL" << endl;

    return new CAcIFFileSystem(inConfiguration);
  }
#else
  return new CAcIFFileSystem(inConfiguration);
#endif
};

/**
 */
CAFInvertedFile::CAFInvertedFile(){
  mName="inverted_file";
};

