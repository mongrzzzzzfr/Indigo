const moduleFn = require('./libindigo-ketcher.js')

const promise = moduleFn({option1: 'test'})
promise.then(indigo => {
    // Input data - map for options and input SMILES
    options = new indigo.map$string$$string$();
    benzene_smiles = "C1=CC=CC=C1"
    // Check options settings - first save molfile v2000, then v3000
    console.log("********* Check options settings - saving molfiles 2000 and 3000 *********")
    options.set('molfile-saving-mode', '3000');
    console.log(indigo.convert(benzene_smiles, "molfile", options));
    options.set('molfile-saving-mode', '2000');
    console.log(indigo.convert(benzene_smiles, "molfile", options));
    // Check exception throwing for wrong input molecule
    console.log("********* Check exception throwing becaue of wrong input molecule *********")
    try {
        console.dir(indigo.convert("C1C2", "molfile", options));
    } catch (e) {
        console.log("Exception: " + e);
    }
    // Check exception throwing for wrong convert format
    console.log("********* Check exception throwing becaue of wrong convert format *********")
    try {
        console.dir(indigo.convert(benzene_smiles, "smils", options));
    } catch (e) {
        console.log("Exception: " + e);
    }
    // Check aromatization and dearomatization
    console.log("********* Check aromatize *********")
    aromatized_molecule = indigo.aromatize(benzene_smiles, options)
    console.log("aromatized: " + indigo.convert(aromatized_molecule, "smiles", options));
    console.log("********* Check dearomatized *********")
    dearomatized_molecule = indigo.dearomatize(aromatized_molecule, options)
    console.log("dearomatized: " + indigo.convert(dearomatized_molecule, "smiles", options));
    // Check layout
    console.log("********* Check layout *********")
    console.log(indigo.layout(benzene_smiles, options));
    // Check clean2d
    console.log("********* Check clean2d *********")
    console.log(indigo.clean2d(benzene_smiles, options));
    // Check automap
    console.log("********* Check automap *********")
    console.log(indigo.convert(indigo.automap("C1=CC=CC=C1.N>>C1=CC=CC=N1.[CH3-]", "discard", options), "smiles", options));
    // Check check
    console.log("********* Check check *********")
    console.log(indigo.check(benzene_smiles, "", options));
    // TODO: Check calculate
    // console.log("********* Check calculate *********")
    // console.log(indigo.calculate(benzene_smiles, options));
    // Check calculateCip
    console.log("********* Check calculateCip *********")
    options.set("ignore-stereochemistry-errors", "true")
    console.log(indigo.calculateCip("CN1C=C(/C=C2/SC(=S)N(CC([O-])=O)C/2=O)C2=CC=CC=C12", options));
})
