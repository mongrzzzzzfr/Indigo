#include <functional>

#include <gtest/gtest.h>

#include <base_cpp/output.h>
#include <base_cpp/profiling.h>
#include <base_cpp/scanner.h>
#include <molecule/cmf_loader.h>
#include <molecule/molecule.h>
#include <molecule/molecule_substructure_matcher.h>
#include <molecule/molfile_loader.h>
#include <molecule/sdf_loader.h>
#include <molecule/smiles_loader.h>

#include <bingo-nosql.h>
#include <indigo.h>

#include "common.h"

using namespace indigo;

TEST(BingoNosqlTest, test_enumerate_id)
{

    int db = bingoCreateDatabaseFile("test.db", "molecule", "");
    int obj = indigoLoadMoleculeFromString("C1CCNCC1");
    bingoInsertRecordObj(db, obj);
    bingoInsertRecordObj(db, obj);
    bingoInsertRecordObj(db, obj);

    int count = 0;
    int e = bingoEnumerateId(db);
    while (bingoNext(e))
    {
        count++;
    }

    bingoEndSearch(e);
    bingoCloseDatabase(db);

    ASSERT_EQ(count, 3);
}

TEST(BingoNosqlTest, test_loadtargetscmf)
{
    FileScanner sc(dataPath("molecules/resonance/resonance.sdf").c_str());

    SdfLoader sdf(sc);
    QueryMolecule qmol;

    Array<char> qbuf;
    qbuf.readString("N(#C)=C(C)C", false);
    BufferScanner sm_scanner(qbuf);
    SmilesLoader smiles_loader(sm_scanner);
    smiles_loader.loadQueryMolecule(qmol);

    sdf.readAt(138);
    try
    {
        BufferScanner bsc(sdf.data);
        MolfileLoader loader(bsc);
        Molecule mol;
        loader.loadMolecule(mol);
        Array<char> buf;
        ArrayOutput buf_out(buf);
        CmfSaver cmf_saver(buf_out);

        cmf_saver.saveMolecule(mol);

        Molecule mol2;
        BufferScanner buf_in(buf);
        CmfLoader cmf_loader(buf_in);
        cmf_loader.loadMolecule(mol2);

        MoleculeSubstructureMatcher matcher(mol2);
        matcher.use_pi_systems_matcher = true;
        matcher.setQuery(qmol);
        matcher.find();
    }
    catch (Exception& e)
    {
        ASSERT_STREQ("", e.message());
    }
}