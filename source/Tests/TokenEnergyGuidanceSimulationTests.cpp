#include "SimulationGpuTest.h"

class TokenEnergyGuidanceSimulationGpuTest
    : public SimulationGpuTest
{
public:
    virtual ~TokenEnergyGuidanceSimulationGpuTest() = default;

protected:
    virtual void SetUp();

    const float tokenTransferEnergyAmount = 10.0;
};

void TokenEnergyGuidanceSimulationGpuTest::SetUp()
{
    _parameters.radiationProb = 0;    //exclude radiation
    _context->setSimulationParameters(_parameters);
}

/**
* Situation: - one cluster with 2 cells with fitting branch number
*			 - first cell has one token with balance cell command
*			 - both cell have same high energy
* Expected result: energy should be transferred from cell to token
*/
TEST_F(TokenEnergyGuidanceSimulationGpuTest, testBalanceCell_highCellEnergy)
{
    auto const valueCell = 100.0f;

    DataDescription origData;
    auto const& cellMaxTokenBranchNumber = _parameters.cellMaxTokenBranchNumber;

    auto cluster = createHorizontalCluster(2, QVector2D{}, QVector2D{}, 0);
    auto& firstCell = cluster.cells->at(0);
    auto& secondCell = cluster.cells->at(1);
    firstCell.tokenBranchNumber = 0;
    secondCell.tokenBranchNumber = 1;
    *firstCell.energy = _parameters.cellMinEnergy + valueCell + 1 + tokenTransferEnergyAmount;
    secondCell.energy = firstCell.energy;
    auto token = createSimpleToken();
    auto& tokenData = *token.data;
    tokenData[Enums::EnergyGuidance::IN] = Enums::EnergyGuidanceIn::BALANCE_CELL;
    tokenData[Enums::EnergyGuidance::IN_VALUE_CELL] = valueCell;
    firstCell.addToken(token);
    origData.addCluster(cluster);

    uint64_t secondCellId = secondCell.id;

    IntegrationTestHelper::updateData(_access, origData);
    IntegrationTestHelper::runSimulation(1, _controller);

    DataDescription newData = IntegrationTestHelper::getContent(_access, { { 0, 0 },{ _universeSize.x, _universeSize.y } });

    ASSERT_EQ(1, newData.clusters->size());
    auto newCluster = newData.clusters->at(0);

    EXPECT_EQ(2, newCluster.cells->size());

    auto const& cellByCellId = IntegrationTestHelper::getCellByCellId(newData);
    auto newCell = cellByCellId.at(secondCellId);
    ASSERT_EQ(1, newCell.tokens->size());

    auto const& newToken = newCell.tokens->at(0);
    EXPECT_EQ(*token.energy + tokenTransferEnergyAmount, *newToken.energy);
    EXPECT_EQ(*secondCell.energy - tokenTransferEnergyAmount, *newCell.energy);

    checkEnergy(origData, newData);
}

/**
* Situation: - one cluster with 2 cells with fitting branch number
*			 - first cell has one token with balance cell command
*			 - both cell have same low energy
*            - token has high energy
* Expected result: energy should be transferred from token to cell
*/
TEST_F(TokenEnergyGuidanceSimulationGpuTest, testBalanceCell_lowCellEnergy)
{
    auto const valueCell = 100.0f;
    auto const valueToken = 30.0f;

    DataDescription origData;
    auto const& cellMaxTokenBranchNumber = _parameters.cellMaxTokenBranchNumber;

    auto cluster = createHorizontalCluster(2, QVector2D{}, QVector2D{}, 0);
    auto& firstCell = cluster.cells->at(0);
    auto& secondCell = cluster.cells->at(1);
    firstCell.tokenBranchNumber = 0;
    secondCell.tokenBranchNumber = 1;
    *firstCell.energy = _parameters.cellMinEnergy + valueCell - 1 + tokenTransferEnergyAmount;
    secondCell.energy = firstCell.energy;
    auto token = createSimpleToken();
    token.energy = _parameters.tokenMinEnergy + valueToken + 1 + tokenTransferEnergyAmount;
    auto& tokenData = *token.data;
    tokenData[Enums::EnergyGuidance::IN] = Enums::EnergyGuidanceIn::BALANCE_CELL;
    tokenData[Enums::EnergyGuidance::IN_VALUE_CELL] = valueCell;
    tokenData[Enums::EnergyGuidance::IN_VALUE_TOKEN] = valueToken;
    firstCell.addToken(token);
    origData.addCluster(cluster);

    uint64_t secondCellId = secondCell.id;

    IntegrationTestHelper::updateData(_access, origData);
    IntegrationTestHelper::runSimulation(1, _controller);

    DataDescription newData = IntegrationTestHelper::getContent(_access, { { 0, 0 },{ _universeSize.x, _universeSize.y } });

    auto const& cellByCellId = IntegrationTestHelper::getCellByCellId(newData);
    auto newCell = cellByCellId.at(secondCellId);

    auto const& newToken = newCell.tokens->at(0);
    EXPECT_EQ(*token.energy - tokenTransferEnergyAmount, *newToken.energy);
    EXPECT_EQ(*secondCell.energy + tokenTransferEnergyAmount, *newCell.energy);

    checkEnergy(origData, newData);
}

/**
* Situation: - one cluster with 2 cells with fitting branch number
*			 - first cell has one token with balance token command
*			 - both cell have same low energy
*            - token has high energy
* Expected result: energy should be transferred from token to cell
*/
TEST_F(TokenEnergyGuidanceSimulationGpuTest, testBalanceToken_highTokenEnergy)
{
    auto const valueToken = 30.0f;

    DataDescription origData;
    auto const& cellMaxTokenBranchNumber = _parameters.cellMaxTokenBranchNumber;

    auto cluster = createHorizontalCluster(2, QVector2D{}, QVector2D{}, 0);
    auto& firstCell = cluster.cells->at(0);
    auto& secondCell = cluster.cells->at(1);
    firstCell.tokenBranchNumber = 0;
    secondCell.tokenBranchNumber = 1;
    auto token = createSimpleToken();
    token.energy = _parameters.tokenMinEnergy + valueToken + 1 + tokenTransferEnergyAmount;
    auto& tokenData = *token.data;
    tokenData[Enums::EnergyGuidance::IN] = Enums::EnergyGuidanceIn::BALANCE_TOKEN;
    tokenData[Enums::EnergyGuidance::IN_VALUE_TOKEN] = valueToken;
    firstCell.addToken(token);
    origData.addCluster(cluster);

    uint64_t secondCellId = secondCell.id;

    IntegrationTestHelper::updateData(_access, origData);
    IntegrationTestHelper::runSimulation(1, _controller);

    DataDescription newData = IntegrationTestHelper::getContent(_access, { { 0, 0 },{ _universeSize.x, _universeSize.y } });

    auto const& cellByCellId = IntegrationTestHelper::getCellByCellId(newData);
    auto newCell = cellByCellId.at(secondCellId);

    auto const& newToken = newCell.tokens->at(0);
    EXPECT_EQ(*token.energy - tokenTransferEnergyAmount, *newToken.energy);
    EXPECT_EQ(*secondCell.energy + tokenTransferEnergyAmount, *newCell.energy);

    checkEnergy(origData, newData);
}

/**
* Situation: - one cluster with 2 cells with fitting branch number
*			 - first cell has one token with balance token command
*			 - both cell have same low energy
*            - token has low energy
* Expected result: energy should be transferred from token to cell
*/
TEST_F(TokenEnergyGuidanceSimulationGpuTest, testBalanceToken_lowTokenEnergy)
{
    auto const valueCell = 100.0f;
    auto const valueToken = 30.0f;

    DataDescription origData;
    auto const& cellMaxTokenBranchNumber = _parameters.cellMaxTokenBranchNumber;

    auto cluster = createHorizontalCluster(2, QVector2D{}, QVector2D{}, 0);
    auto& firstCell = cluster.cells->at(0);
    auto& secondCell = cluster.cells->at(1);
    firstCell.tokenBranchNumber = 0;
    secondCell.tokenBranchNumber = 1;
    *firstCell.energy = _parameters.cellMinEnergy + valueCell + 1 + tokenTransferEnergyAmount;
    secondCell.energy = firstCell.energy;
    auto token = createSimpleToken();
    token.energy = _parameters.tokenMinEnergy + valueToken - 1 + tokenTransferEnergyAmount;
    auto& tokenData = *token.data;
    tokenData[Enums::EnergyGuidance::IN] = Enums::EnergyGuidanceIn::BALANCE_TOKEN;
    tokenData[Enums::EnergyGuidance::IN_VALUE_CELL] = valueCell;
    tokenData[Enums::EnergyGuidance::IN_VALUE_TOKEN] = valueToken;
    firstCell.addToken(token);
    origData.addCluster(cluster);

    uint64_t secondCellId = secondCell.id;

    IntegrationTestHelper::updateData(_access, origData);
    IntegrationTestHelper::runSimulation(1, _controller);

    DataDescription newData = IntegrationTestHelper::getContent(_access, { { 0, 0 },{ _universeSize.x, _universeSize.y } });

    auto const& cellByCellId = IntegrationTestHelper::getCellByCellId(newData);
    auto newCell = cellByCellId.at(secondCellId);

    auto const& newToken = newCell.tokens->at(0);
    EXPECT_EQ(*token.energy + tokenTransferEnergyAmount, *newToken.energy);
    EXPECT_EQ(*secondCell.energy - tokenTransferEnergyAmount, *newCell.energy);

    checkEnergy(origData, newData);
}
