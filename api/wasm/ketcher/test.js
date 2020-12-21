const moduleFn = require('./libindigo-ketcher.js')

const promise = moduleFn({option1: 'test'})
promise.then(indigo => {
    // Check version
    console.log("********* Check version() *********")
    console.log(indigo.version());
    // Input data - map for options and input SMILES
    options = new indigo.map$string$$string$();
    mol_smiles = "C1=CC=CC=C1"
    qmol_smarts = "[$([NX1-]=[NX2+]=[NX1-]),$([NX1]#[NX2+]-[NX1-2])]"
    rxn_smiles = "C1=CC=CC=C1.N>>C1=CC=CC=N1.[CH3-]"
    // Check convert and options settings - first save molfile v2000, then v3000
    console.log("********* Check options settings - saving molfiles 2000 and 3000 *********")
    options.set('molfile-saving-mode', '3000');
    console.log(indigo.convert(mol_smiles, "molfile", options));
    console.log(indigo.convert(rxn_smiles, "rxnfile", options));
    options.set('molfile-saving-mode', '2000');
    console.log(indigo.convert(rxn_smiles, "rxnfile", options));
    console.log(indigo.convert(mol_smiles, "molfile", options));
    console.log(indigo.convert(mol_smiles, "smiles", options));
    console.log(indigo.convert(rxn_smiles, "smiles", options));
    console.log(indigo.convert(qmol_smarts, "smarts", options));
    console.log(indigo.convert(mol_smiles, "cml", options));
    console.log(indigo.convert(rxn_smiles, "cml", options));
    console.log(indigo.convert(mol_smiles, "inchi", options));
    console.log(indigo.convert(mol_smiles, "inchi-aux", options));
    // Check exception throwing for wrong input molecule
    console.log("********* Check exception throwing because of wrong input molecule *********")
    try {
        console.dir(indigo.convert("C1C2", "molfile", options));
    } catch (e) {
        console.log("Exception: " + e);
    }
    // Check exception throwing for wrong convert format
    console.log("********* Check exception throwing becaue of wrong convert format *********")
    try {
        console.dir(indigo.convert(mol_smiles, "smils", options));
    } catch (e) {
        console.log("Exception: " + e);
    }
    // Check aromatization and dearomatization
    console.log("********* Check aromatize *********")
    aromatized_molecule = indigo.aromatize(mol_smiles, options)
    console.log("aromatized: " + indigo.convert(aromatized_molecule, "smiles", options));
    console.log("********* Check dearomatized *********")
    dearomatized_molecule = indigo.dearomatize(aromatized_molecule, options)
    console.log("dearomatized: " + indigo.convert(dearomatized_molecule, "smiles", options));
    try {
        dearomatized_molecule = indigo.dearomatize(qmol_smarts, options);
    } catch (e) {
        console.log("Exception: " + e);
    }
    // Check layout
    console.log("********* Check layout *********")
    console.log(indigo.layout(mol_smiles, options));
    // Check clean2d
    console.log("********* Check clean2d *********")
    console.log(indigo.clean2d(mol_smiles, options));
    // Check automap
    console.log("********* Check automap *********")
    console.log(indigo.convert(indigo.automap(rxn_smiles, "discard", options), "smiles", options));
    // Check check
    console.log("********* Check check *********")
    console.log(indigo.check(mol_smiles, "", options));
    // Check calculateCip
    console.log("********* Check calculateCip *********")
    options.set("ignore-stereochemistry-errors", "true")
    console.log(indigo.calculateCip("CN1C=C(/C=C2/SC(=S)N(CC([O-])=O)C/2=O)C2=CC=CC=C12", options));
    // Check calculate
    console.log("********* Check calculate *********")
    console.log(indigo.calculate(mol_smiles, options));
})
