#include "TranslateModule.h"

static string _TranslateModule = RegisterPlugin("Translate", new TranslateModule());

TranslateModule::TranslateModule() {}

CASP_Return* TranslateModule::Execute(Markup* markup, LanguageDescriptorObject* source_ldo, vector<arg> fnArgs) {

    cout << "This is the entry point for the " << _TranslateModule << " Module!\n";

    this->source_ldo = source_ldo;
    string targetLanguage = Helpers::ParseArgument("targetlang", fnArgs);
    ReadLanguageFile(targetLanguage);

    Translate(markup);

    return NULL;

}

string Translate(Markup* markup) {

    markup* targetRoot = new Markup("ROOT");
    string nodeId = markup->GetID();

    vector<Markup*> children = markup->Children();
    for (int i = 0; i < children.size(); i++) {
        MatchTargetProd(children[i]);
    }

}

Markup* MatchTargetProd(Markup* markup) {

    string nodeId = markup->GetID();
    markup* targetProd = target_ldo->findProdById(nodeId);

    if (targetProd != NULL) {
        cout << "Production " << nodeId << " was matched\n";
        return TranslateProd(markup, targetProd);
    } else {
        // vector<Markup*> children = markup->Children();
        // for (int i = 0; i < children.size(); i++) {
        //     Translate(children[i]);
        // }
    }
}

Markup* TranslateMarkup(Markup* source, Production* target) {
    // vector<Markup*> targetChildren = markup->Children();
    // for (int i = 0; i < targetChildren.size(); i++) {
    //     if (targetChildren[i]->IsLeaf()) {
            
    //     }
    // }
}


void TranslateModule::ReadLanguageFile(string targetLanguage) {
    try {
        // read and parse file;
        target_ldo = new LanguageDescriptorObject(targetLanguage);
    } catch (...) {
        string err = "Language '" + targetLanguage + "' could not be read";
        cout << err << endl;
        throw err;
    }
}