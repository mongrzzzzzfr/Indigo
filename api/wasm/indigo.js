const libindigo = require('./libindigo.js');

// Wrap low-level C functions

_indigoGetLastError = libindigo.cwrap('indigoGetLastError', 'string', [])
_indigoAromatize = libindigo.cwrap('indigoAromatize', 'number', ['number'])
_indigoDearomatize = libindigo.cwrap('indigoDearomatize', 'number', ['number'])
_indigoVersion = libindigo.cwrap('indigoVersion', 'string', [])
_indigoLoadMoleculeFromString = libindigo.cwrap('indigoLoadMoleculeFromString', 'number', ['string'])
_indigoMolfile = libindigo.cwrap('indigoMolfile', 'string', ['number'])
_indigoSmiles = libindigo.cwrap('indigoSmiles', 'string', ['number'])

// High level wrappers

class IndigoException {
    constructor(message) {
        this.message = message;
        this.stack = (new Error).stack;
    }
}

class IndigoObject {
    constructor(_id, _indigo) {
        this._id = _id;
        this._indigo = _indigo;
    }

    aromatize() {
        this._indigo._checkResult(_indigoAromatize(this._id));
    }

    dearomatize() {
        this._indigo._checkResult(_indigoDearomatize(this._id));
    }

    molfile() {
        return this._indigo._checkResultString(_indigoMolfile(this._id));
    }

    smiles() {
        return this._indigo._checkResultString(_indigoSmiles(this._id));
    }
}

class Indigo {
    loadMoleculeFromString(s) {
        return new IndigoObject(this._checkResult(_indigoLoadMoleculeFromString(s)), this);
    }

    loadMolecule(s) {
        return this.loadMoleculeFromString(s);
    }

    version() {
        this._checkResultString(_indigoVersion);
    }

    /*
     * Get the last error message
     *
     * @method getLastError
     */
    _getLastError() {
        return _indigoGetLastError();
    };

    /*
     * Check result
     *
     * @method _checkResult
     * @param {number} result
     */
    _checkResult(result) {
        if (result < 0) {
            throw new IndigoException(this._getLastError());
        }
        return result;
    };

    /*
     * Check result
     *
     * @method _checkResultFloat
     * @param {number} result
     */
    _checkResultFloat(result) {
        if (result < -0.5) {
            throw new IndigoException(this._getLastError());
        }
        return result;
    };

    /*
     * Check string result
     *
     * @method _checkResultString
     * @param {string} result
     */
    _checkResultString(result) {
        if (typeof result !== 'string') {
            throw new IndigoException(this._getLastError());
        }
        return result;
    }
}

// Some simple tests

let i = new Indigo();
let m = i.loadMolecule("C1=CC=CC=C1");
console.log('Aromatizing...');
m.aromatize();
console.log(m.smiles());
console.log('Dearomatizing...');
m.dearomatize()
console.log(m.smiles());

let m2 = i.loadMoleculeFromString('C1C2');
// try {
//     letm2 = i.loadMoleculeFromString('C1C2');
// } catch (error) {
//     console.error("exception cathed: " + error); // TODO: Exception is being catched, but for some reason it's a number, so no error message yet
// }
