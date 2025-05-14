// Copyright Epic Games, Inc. All Rights Reserved.

#include "GAM415ReabanProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMathlibrary.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"

AGAM415ReabanProjectile::AGAM415ReabanProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AGAM415ReabanProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Create BallMesh to use

	ballMesh = CreateDefaultSubobject<UStaticMeshComponent>("Ball Mesh");

	// Set as root component
	RootComponent = CollisionComp;

	// Attach ballmesh to root

	ballMesh->SetupAttachment(CollisionComp);

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

void AGAM415ReabanProjectile::BeginPlay()
{
	Super::BeginPlay();
	float ranNumX = UKismetMathLibrary::RandomFloatInRange(0.f, 1.f);
	float ranNumY = UKismetMathLibrary::RandomFloatInRange(0.f, 1.f);
	float ranNumZ = UKismetMathLibrary::RandomFloatInRange(0.f, 1.f);

	randColor = FLinearColor(ranNumX, ranNumY, ranNumZ, 1.0f);

	dmiMat = UMaterialInstanceDynamic::Create(projMat, this);
	
	ballMesh->SetMaterial(0, dmiMat);

	
	dmiMat->SetVectorParameterValue("ProjColor", randColor);
}

void AGAM415ReabanProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

		Destroy();
	}

	if (OtherActor != nullptr)
	{
	
		// Random Frame Number
		float ranNumFrame = UKismetMathLibrary::RandomFloatInRange(0.f, 3.f);

		

		// This is the size
		FVector decalVector;

		//decalVector = FVector(UKismetMathLibrary::RandomFloatInRange(20.f, 40.f));
		
		auto Decal = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), baseMat, FVector(UKismetMathLibrary::RandomFloatInRange(20.f, 40.f)), Hit.Location, Hit.Normal.Rotation(), 0.f);
		
		//auto Decal = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), baseMat, FVector(UKismetMathLibrary::RandomFloatInRange(20.f, 40.f)), Hit.Location, Hit.Normal.Rotation(), 0.f);
		
		auto MatInstance = Decal->CreateDynamicMaterialInstance();

		MatInstance->SetVectorParameterValue("Color", randColor);
		MatInstance->SetScalarParameterValue("Frame", ranNumFrame);
	}

}