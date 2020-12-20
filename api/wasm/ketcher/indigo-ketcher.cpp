#include <map>
#include <sstream>
#include <string>
#include <vector>

#include <emscripten.h>
#include <emscripten/bind.h>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>

#include "indigo-inchi.h"
#include "indigo.h"

namespace indigo
{
    using cstring = const char*;

    EM_JS(void, jsThrow, (cstring str), {
        throw UTF8ToString(str);
    });

    int _checkResult(int result)
    {
        if (result < 0)
        {
            jsThrow(indigoGetLastError());
        }
        return result;
    }

    double _checkResultFloat(double result)
    {
        if (result < 0.5)
        {
            jsThrow(indigoGetLastError());
        }
        return result;
    }

    cstring _checkResultString(cstring result)
    {
        if (!result)
        {
            jsThrow(indigoGetLastError());
        }
        return result;
    }

    struct IndigoKetcherObject
    {
        int id;
        bool isReaction;
        bool isQuery;

        ~IndigoKetcherObject()
        {
            indigoFree(id);
        }

        std::string toString() const
        {
            if (isReaction)
            {
                return _checkResultString(indigoRxnfile(id));
            }
            return _checkResultString(indigoMolfile(id));
        }
    };

    void indigoSetOptions(const std::map<std::string, std::string>& options)
    {
        for (const auto& option : options)
        {
            if (option.first != "smiles")
            {
                _checkResult(indigoSetOption(option.first.c_str(), option.second.c_str()));
            }
        }
    }

    IndigoKetcherObject loadMoleculeOrReaction(cstring data)
    {
        std::vector<std::string> exceptionMessages;
        // Let's try a simple molecule
        int objectId = indigoLoadMoleculeFromString(data);
        if (objectId >= 0)
        {
            return {objectId, false, false};
        }
        exceptionMessages.emplace_back(indigoGetLastError());
        // Let's try query molecule
        objectId = indigoLoadQueryMoleculeFromString(data);
        if (objectId >= 0)
        {
            return {objectId, false, true};
        }
        exceptionMessages.emplace_back(indigoGetLastError());
        // Let's try simple reaction
        objectId = indigoLoadReactionFromString(data);
        if (objectId >= 0)
        {
            return {objectId, true, false};
        }
        exceptionMessages.emplace_back(indigoGetLastError());
        // Let's try query reaction
        objectId = indigoLoadQueryReactionFromString(data);
        if (objectId >= 0)
        {
            return {objectId, true, true};
        }
        exceptionMessages.emplace_back(indigoGetLastError());
        // It's not anything we can load, let's throw an exception
        std::stringstream ss;
        ss << "Given string could not be loaded as (query or plain) molecule or reaction, see the error messages: ";
        for (const auto& exceptionMessage : exceptionMessages)
        {
            ss << "'" << exceptionMessage << "', ";
        }
        std::string exceptionText = ss.str();
        exceptionText.pop_back();
        exceptionText.pop_back();
        jsThrow(exceptionText.c_str());
    }

    std::string version()
    {
        return indigoVersion();
    }

    std::string convert(const std::string& data, const std::string& outputFormat, const std::map<std::string, std::string>& options)
    {
        indigoSetOptions(options);
        IndigoKetcherObject iko = loadMoleculeOrReaction(data.c_str());
        if (outputFormat == "molfile" || outputFormat == "rxnfile" || outputFormat == "chemical/x-mdl-molfile" || outputFormat == "chemical/x-mdl-rxnfile")
        {
            if (iko.isReaction)
            {
                return indigoRxnfile(iko.id);
            }
            return indigoMolfile(iko.id);
            }
        if (outputFormat == "smiles" or outputFormat == "chemical/x-daylight-smiles" || outputFormat == "chemical/x-chemaxon-cxsmiles")
        {
            if (options.count("smiles") && options.at("smiles") == "canonical")
            {
                return indigoCanonicalSmiles(iko.id);
            }
            return indigoCanonicalSmiles(iko.id);
            }
        if (outputFormat == "smarts" || outputFormat == "chemical/x-daylight-smarts")
        {
            if (options.count("smarts") && options.at("smarts") == "canonical")
            {
                return indigoCanonicalSmarts(iko.id);
            }
            return indigoSmarts(iko.id);
            }
        if (outputFormat == "cml" || outputFormat == "chemical/x-cml")
        {
            return indigoCml(iko.id);
        }
        if (outputFormat == "inchi" || outputFormat == "chemical/x-inchi")
        {
            return indigoInchiGetInchi(iko.id);
        }
        if (outputFormat == "inchi-aux" || outputFormat == "chemical/x-inchi-aux")
        {
            std::stringstream ss;
            ss << indigoInchiGetInchi(iko.id) << '\n' << indigoInchiGetAuxInfo();
            return ss.str();
        }
        std::stringstream ss;
        ss << "Unknown output format: " << outputFormat;
        jsThrow(ss.str().c_str());
    }

    std::string aromatize(const std::string& data, const std::map<std::string, std::string>& options)
    {
        indigoSetOptions(options);
        const auto iko = loadMoleculeOrReaction(data.c_str());
        _checkResult(indigoAromatize(iko.id));
        return iko.toString();
    }

    std::string dearomatize(const std::string& data, const std::map<std::string, std::string>& options)
    {
        indigoSetOptions(options);
        const auto iko = loadMoleculeOrReaction(data.c_str());
        _checkResult(indigoDearomatize(iko.id));
        return iko.toString();
    }

    std::string layout(const std::string& data, const std::map<std::string, std::string>& options)
    {
        indigoSetOptions(options);
        const auto iko = loadMoleculeOrReaction(data.c_str());
        _checkResult(indigoLayout(iko.id));
        return iko.toString();
    }

    std::string clean2d(const std::string& data, const std::map<std::string, std::string>& options)
    {
        indigoSetOptions(options);
        const auto iko = loadMoleculeOrReaction(data.c_str());
        _checkResult(indigoClean2d(iko.id));
        return iko.toString();
    }

    std::string automap(const std::string& data, const std::string& mode, const std::map<std::string, std::string>& options)
    {
        indigoSetOptions(options);
        const auto iko = loadMoleculeOrReaction(data.c_str());
        _checkResult(indigoAutomap(iko.id, mode.c_str()));
        return iko.toString();
    }

    std::string check(const std::string& data, const std::string& properties, const std::map<std::string, std::string>& options)
    {
        indigoSetOptions(options);
        const auto iko = loadMoleculeOrReaction(data.c_str());
        return _checkResultString(indigoCheck(iko.id, properties.c_str()));
    }

    std::string calculate(const std::string& data, const std::map<std::string, std::string>& options)
    {
        indigoSetOptions(options);
        indigoSetOption("molfile-saving-add-stereo-desc", "true");
        const auto iko = loadMoleculeOrReaction(data.c_str());
        rapidjson::Document result;
        result["molecular-weight"] = _checkResultFloat(indigoMolecularWeight(iko.id));
        result["most-abundant-mass"] = _checkResultFloat(indigoMostAbundantMass(iko.id));
        result["monoisotopic-mass"] = _checkResultFloat(indigoMonoisotopicMass(iko.id));
        {
            const std::string massComposition =_checkResultString(indigoMassComposition(iko.id));
            result["mass-composition"].SetString(massComposition.c_str(), massComposition.size());
        }
        {
            const std::string grossFormula = _checkResultString(indigoToString(_checkResult(indigoGrossFormula(iko.id))));
            result["gross"].SetString(grossFormula.c_str(), grossFormula.size());
        }
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        result.Accept(writer);
        return buffer.GetString();
    }

    std::string calculateCip(const std::string& data, const std::map<std::string, std::string>& options)
    {
        indigoSetOptions(options);
        indigoSetOption("molfile-saving-add-stereo-desc", "true");
        const auto iko = loadMoleculeOrReaction(data.c_str());
        return iko.toString();
    }

#ifdef __EMSCRIPTEN__

    EMSCRIPTEN_BINDINGS(module)
    {
        emscripten::function("version", &version);
        emscripten::function("convert", &convert);
        emscripten::function("aromatize", &aromatize);
        emscripten::function("dearomatize", &dearomatize);
        emscripten::function("layout", &layout);
        emscripten::function("clean2d", &clean2d);
        emscripten::function("automap", &automap);
        emscripten::function("check", &check);
        // TODO: Check calculate // emscripten::function("calculate", &calculate);
        emscripten::function("calculateCip", &calculateCip);

        // register_map<std::string, std::string>("map<string, string>");
        emscripten::register_map<std::string, std::string>("map<string, string>");
    }

#endif

} // namespace indigo
