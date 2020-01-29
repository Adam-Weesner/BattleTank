// Adam Weesner 2020


#include "SprungWheel.h"

// Sets default values
ASprungWheel::ASprungWheel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PostPhysics;

	SpringConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(FName("Spring Constraint"));
	SetRootComponent(SpringConstraint);

	Axle = CreateDefaultSubobject<USphereComponent>(FName("Axle"));
	Axle->SetupAttachment(RootComponent);

	AxleContraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(FName("Axle Constraint"));
	AxleContraint->SetupAttachment(Axle);

	Wheel = CreateDefaultSubobject<USphereComponent>(FName("Wheel"));
	Wheel->SetupAttachment(Axle);
}


// Called when the game starts or when spawned
void ASprungWheel::BeginPlay()
{
	Super::BeginPlay();

	Wheel->SetNotifyRigidBodyCollision(true);
	Wheel->OnComponentHit.AddDynamic(this, &ASprungWheel::OnHit);

	SetupConstraints();

	return;
}


// Called every frame
void ASprungWheel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Double-check to make sure when in the correct tick group (after the physics)
	if (GetWorld()->TickGroup == TG_PostPhysics)
	{
		// Cancel force before next frame
		ForceMagnitudeThisFrame = 0.0f;
	}
}


void ASprungWheel::SetupConstraints()
{
	if (!GetAttachParentActor()) return;

	UPrimitiveComponent* BodyRoot = Cast<UPrimitiveComponent>(GetAttachParentActor()->GetRootComponent());

	if (!BodyRoot) return;

	// Body to Axle constraint
	SpringConstraint->SetConstrainedComponents(
		BodyRoot,
		NAME_None,
		Axle,
		NAME_None
	);

	// Axle to wheel constraint
	AxleContraint->SetConstrainedComponents(
		Axle,
		NAME_None,
		Wheel,
		NAME_None
	);
}


void ASprungWheel::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("Onhit %f"), GetWorld()->GetDeltaSeconds());
	ApplyForce();
}


void ASprungWheel::ApplyForce()
{
	Wheel->AddForce(Axle->GetForwardVector() * ForceMagnitudeThisFrame);
}

void ASprungWheel::AddDrivingForce(float NewForceMagnitude)
{
	ForceMagnitudeThisFrame += NewForceMagnitude;
}

