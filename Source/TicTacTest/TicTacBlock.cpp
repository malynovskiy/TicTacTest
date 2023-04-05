#include "TicTacBlock.h"
#include "TicTacBoard.h"

ATicTacBlock::ATicTacBlock()
{
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> PlaneMesh;
		ConstructorHelpers::FObjectFinderOptional<UMaterial> BaseMaterial;
    ConstructorHelpers::FObjectFinderOptional<UMaterial> Player1FocusedMaterial;
    ConstructorHelpers::FObjectFinderOptional<UMaterial> Player2FocusedMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterial> X_Material;
		ConstructorHelpers::FObjectFinderOptional<UMaterial> O_Material;
		FConstructorStatics()
			: PlaneMesh(TEXT("/Game/Meshes/Cube.Cube"))
			, BaseMaterial(TEXT("/Game/Materials/BaseTicTac.BaseTicTac"))
			, Player1FocusedMaterial(TEXT("/Game/Materials/Player1FocusedTicTacMaterial.Player1FocusedTicTacMaterial"))
			, Player2FocusedMaterial(TEXT("/Game/Materials/Player2FocusedTicTacMaterial.Player2FocusedTicTacMaterial"))
			, X_Material(TEXT("/Game/Materials/X_Material.X_Material"))
			, O_Material(TEXT("/Game/Materials/O_Material.O_Material"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	// Create dummy root scene component
  /*DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
  RootComponent = DummyRoot;*/

	// Create static mesh component
	BlockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockMesh0"));
	BlockMesh->SetStaticMesh(ConstructorStatics.PlaneMesh.Get());

	BlockMesh->SetRelativeScale3D(FVector(2.5f, 2.5f, 2.5f));
  BlockMesh->SetRelativeLocation(FVector(0.f, 0.f, 25.f));
  BlockMesh->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));

	BlockMesh->SetMaterial(0, ConstructorStatics.BaseMaterial.Get());
	//BlockMesh->SetupAttachment(DummyRoot);
	BlockMesh->OnClicked.AddDynamic(this, &ATicTacBlock::BlockClicked);

	BaseMaterial = ConstructorStatics.BaseMaterial.Get();
	Player1FocusedMaterial = ConstructorStatics.Player1FocusedMaterial.Get();
	Player2FocusedMaterial = ConstructorStatics.Player2FocusedMaterial.Get();
	X_Material = ConstructorStatics.X_Material.Get();
	O_Material = ConstructorStatics.O_Material.Get();
}

void ATicTacBlock::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATicTacBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATicTacBlock::BlockClicked(UPrimitiveComponent* ClickedComp, FKey ButtonClicked)
{
	HandleClicked();
}

void ATicTacBlock::HandleClicked()
{
	// prevent clicking on already occupied blocks
	if (bIsActive)
		return;
	
	bIsActive = true;

  APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
  if (PlayerController == nullptr)
  {
    UE_LOG(LogTemp, Error, TEXT("PlayerController is null"));
    return;
  }

  ATicTacPawn* ticTacPawn = dynamic_cast<ATicTacPawn*>(PlayerController->GetPawn());
  if (ticTacPawn == nullptr)
  {
    UE_LOG(LogTemp, Error, TEXT("ATicTacPawn is null"));
    return;
  }

	const TicTacTest::Player currentPlayer = ticTacPawn->GetCurrentPlayer();

	if (currentPlayer == TicTacTest::Player::Player1)
	{
		BlockMesh->SetMaterial(0, X_Material);
	}
	else
	{
		BlockMesh->SetMaterial(0, O_Material);
	}

  if (OwningGrid != nullptr)
  {
    OwningGrid->HandlePlayerMove(index, currentPlayer);
  }
}

void ATicTacBlock::Highlight(bool bOn, TicTacTest::Player currentPlayer)
{
	if (bIsActive)
		return;

	if (!bOn)
		BlockMesh->SetMaterial(0, BaseMaterial);
	else
		BlockMesh->SetMaterial(0, currentPlayer == TicTacTest::Player::Player1 ? Player1FocusedMaterial : Player2FocusedMaterial);
}
