#include <gtest/gtest.h>

#include <indigo_internal.h>

class IndigoSimilarityTest : public ::testing::Test
{
public:
    qword session;
    Indigo& indigo_instance = indigoGetInstance();

    int m1;
    int m2; // `m1` with a small chain
    int m3; // `m2` written in a non-canonical way
    int m4; // contains explicit hydrogen atoms

protected:
    void SetUp() override
    {
        session = indigoAllocSessionId();
        indigoSetSessionId(session);

        m1 = indigoLoadMoleculeFromString("C1C=C(OCC)C=CC=1");
        m2 = indigoLoadMoleculeFromString("C1C=C(OCCCC)C=CC=1");
        m3 = indigoLoadMoleculeFromString("C1=CC=C(OCCCC)C=C1");
        m4 = indigoLoadMoleculeFromString("Cl[2H].OC(=O)[C@@H](N)CC1=CC([2H])=C(O)C([2H])=C1");
    }
};

TEST_F(IndigoSimilarityTest, generate_fingerprints)
{
    for (int mol : {m1, m2, m4})
    {
        for (auto& type : {"sim", "sub", "sub-res", "sub-tau", "full"})
        {
            ASSERT_NE(-1, indigoFingerprint(mol, type));
        }

        const char* similarity_type_names[] = {
            "sim",   "chem", "ecfp2", "ecfp4",
            "ecfp6", "ecfp8"

            //      TODO: implement FCFP fingerprints
            //            "fcfp2", "fcfp4", "fcfp6", "fcfp8"
        };

        for (auto& mode : similarity_type_names)
        {
            indigo_instance.fp_params.similarity_type = MoleculeFingerprintBuilder::parseSimilarityType(mode);
            ASSERT_NE(-1, indigoFingerprint(mol, "sim"));
        }
    }
}

TEST_F(IndigoSimilarityTest, generate_fingerprints_sim_modes)
{
    for (int mol : {m1, m2, m4})
    {
        for (auto simType :
             {SimilarityType::SIM, SimilarityType::CHEM, SimilarityType::ECFP2, SimilarityType::ECFP4, SimilarityType::ECFP6, SimilarityType::ECFP8})
        {
            indigo_instance.fp_params.similarity_type = simType;
            ASSERT_NE(-1, indigoFingerprint(mol, "sim"));
        }
    }
}

TEST_F(IndigoSimilarityTest, similarity_sim)
{
    const char* type = "sim";
    int f1 = indigoFingerprint(m1, type);
    int f2 = indigoFingerprint(m2, type);
    int f3 = indigoFingerprint(m3, type);

    ASSERT_LT(0.85, indigoSimilarity(f1, f2, "tanimoto"));
    ASSERT_EQ(1.00, indigoSimilarity(f2, f3, "tanimoto"));
}

TEST_F(IndigoSimilarityTest, similarity_sub)
{
    const char* type = "sub";
    int f1 = indigoFingerprint(m1, type);
    int f2 = indigoFingerprint(m2, type);
    int f3 = indigoFingerprint(m3, type);

    ASSERT_LT(0.85, indigoSimilarity(f1, f2, "tanimoto"));
    ASSERT_EQ(1.00, indigoSimilarity(f2, f3, "tanimoto"));
}

TEST_F(IndigoSimilarityTest, similarity_subres)
{
    const char* type = "sub-res";
    int f1 = indigoFingerprint(m1, type);
    int f2 = indigoFingerprint(m2, type);
    int f3 = indigoFingerprint(m3, type);

    ASSERT_LT(0.85, indigoSimilarity(f1, f2, "tanimoto"));
    ASSERT_EQ(1.00, indigoSimilarity(f2, f3, "tanimoto"));
}

TEST_F(IndigoSimilarityTest, similarity_subtau)
{
    const char* type = "sub-tau";
    int f1 = indigoFingerprint(m1, type);
    int f2 = indigoFingerprint(m2, type);
    int f3 = indigoFingerprint(m3, type);

    ASSERT_LT(0.85, indigoSimilarity(f1, f2, "tanimoto"));
    ASSERT_EQ(1.00, indigoSimilarity(f2, f3, "tanimoto"));
}

TEST_F(IndigoSimilarityTest, similarity_full)
{
    const char* type = "full";
    int f1 = indigoFingerprint(m1, type);
    int f2 = indigoFingerprint(m2, type);
    int f3 = indigoFingerprint(m3, type);

    ASSERT_LT(0.85, indigoSimilarity(f1, f2, "tanimoto"));
    ASSERT_EQ(1.00, indigoSimilarity(f2, f3, "tanimoto"));
}

TEST_F(IndigoSimilarityTest, similarity_normilized_edit)
{
    const char* type = "full";
    int f1 = indigoFingerprint(m1, type);
    int f2 = indigoFingerprint(m2, type);

    ASSERT_LT(0.80, indigoSimilarity(m1, m2, "normalized-edit"));
    ASSERT_EQ(1.00, indigoSimilarity(m2, m3, "normalized-edit"));
    ASSERT_EQ(-1, indigoSimilarity(f1, f2, "normalized-edit"));
}

TEST_F(IndigoSimilarityTest, similarity_sim_chem_mode)
{
    const char* type = "sim";
    SimilarityType similarityType = MoleculeFingerprintBuilder::parseSimilarityType("CHEM");
    indigo_instance.fp_params.similarity_type = similarityType;
    int f1 = indigoFingerprint(m1, type);
    int f2 = indigoFingerprint(m2, type);
    int f3 = indigoFingerprint(m3, type);

    ASSERT_LT(0.80, indigoSimilarity(f1, f2, "tanimoto"));
    ASSERT_GT(0.99, indigoSimilarity(f1, f2, "tanimoto"));
    ASSERT_EQ(1.00, indigoSimilarity(f2, f3, "tanimoto"));
}

TEST_F(IndigoSimilarityTest, similarity_sim_ECFP2_mode)
{
    const char* type = "sim";
    SimilarityType similarityType = MoleculeFingerprintBuilder::parseSimilarityType("ECFP2");
    indigo_instance.fp_params.similarity_type = similarityType;
    int f1 = indigoFingerprint(m1, type);
    int f2 = indigoFingerprint(m2, type);
    int f3 = indigoFingerprint(m3, type);

    ASSERT_LT(0.80, indigoSimilarity(f1, f2, "tanimoto"));
    ASSERT_GT(0.99, indigoSimilarity(f1, f2, "tanimoto"));
    ASSERT_EQ(1.00, indigoSimilarity(f2, f3, "tanimoto"));
}

TEST_F(IndigoSimilarityTest, similarity_sim_ECFP4_mode)
{
    const char* type = "sim";
    SimilarityType similarityType = MoleculeFingerprintBuilder::parseSimilarityType("ECFP4");
    indigo_instance.fp_params.similarity_type = similarityType;
    int f1 = indigoFingerprint(m1, type);
    int f2 = indigoFingerprint(m2, type);
    int f3 = indigoFingerprint(m3, type);

    ASSERT_LT(0.80, indigoSimilarity(f1, f2, "tanimoto"));
    ASSERT_GT(0.99, indigoSimilarity(f1, f2, "tanimoto"));
    ASSERT_EQ(1.00, indigoSimilarity(f2, f3, "tanimoto"));
}

TEST_F(IndigoSimilarityTest, similarity_sim_ECFP6_mode)
{
    const char* type = "sim";
    SimilarityType similarityType = MoleculeFingerprintBuilder::parseSimilarityType("ECFP6");
    indigo_instance.fp_params.similarity_type = similarityType;
    int f1 = indigoFingerprint(m1, type);
    int f2 = indigoFingerprint(m2, type);
    int f3 = indigoFingerprint(m3, type);

    ASSERT_LT(0.80, indigoSimilarity(f1, f2, "tanimoto"));
    ASSERT_GT(0.99, indigoSimilarity(f1, f2, "tanimoto"));
    ASSERT_EQ(1.00, indigoSimilarity(f2, f3, "tanimoto"));
}

TEST_F(IndigoSimilarityTest, similarity_sim_ECFP8_mode)
{
    const char* type = "sim";
    SimilarityType similarityType = MoleculeFingerprintBuilder::parseSimilarityType("ECFP8");
    indigo_instance.fp_params.similarity_type = similarityType;
    int f1 = indigoFingerprint(m1, type);
    int f2 = indigoFingerprint(m2, type);
    int f3 = indigoFingerprint(m3, type);

    ASSERT_LT(0.80, indigoSimilarity(f1, f2, "tanimoto"));
    ASSERT_GT(0.99, indigoSimilarity(f1, f2, "tanimoto"));
    ASSERT_EQ(1.00, indigoSimilarity(f2, f3, "tanimoto"));
}
