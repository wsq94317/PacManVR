// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionWidget.h"
#include "Components/Button.h"
#include "AOfficer.h"



void UInteractionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CommunicateState = ECommunicateState::Free;

	UButton* InteractionButton = Cast<UButton>(GetWidgetFromName(TEXT("InteractionButton")));

	if (InteractionButton)
	{
		InteractionButton->OnClicked.AddDynamic(this, &UInteractionWidget::OnInteractionButtonClicked);
		// InteractionButton->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("InteractionButton not found!"));
	}

	UButton* EndInteractionButton = Cast<UButton>(GetWidgetFromName(TEXT("EndInteractionButton")));

	if (EndInteractionButton)
	{
		EndInteractionButton->OnClicked.AddDynamic(this, &UInteractionWidget::OnEndInteractionButtonClicked);
		// EndInteractionButton->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("EndInteractionButton not found!"));
	}

	SwitchInteractingButtonVisibility(false);

	SwitchInteractionMethodButtonsVisibility(false);

	UButton* VoiceMessageButton = Cast<UButton>(GetWidgetFromName("VoiceMessageButton"));

	if (VoiceMessageButton)
	{
		VoiceMessageButton->OnClicked.AddDynamic(this, &UInteractionWidget::OnVoiceMessageButtonClicked);
		// VoiceMessageButton->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("VoiceMessageButton not found!"));
	
	}

	UButton* VideoMessageButton = Cast<UButton>(GetWidgetFromName("VideoMessageButton"));

	if (VideoMessageButton)
	{
		VideoMessageButton->OnClicked.AddDynamic(this, &UInteractionWidget::OnVideoMessageButtonCLicked);
		// VideoMessageButton->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("VideoMessageButton not found!"));
	
	}

	UButton* SendVoiceButton = Cast<UButton>(GetWidgetFromName("SendVoiceButton"));

	if (SendVoiceButton)
	{
		SendVoiceButton->OnClicked.AddDynamic(this, &UInteractionWidget::OnSendVoiceButtonClicked);
		SendVoiceButton->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("SendVoiceButton not found!"));
	
	}

	UButton* SendVideoButton = Cast<UButton>(GetWidgetFromName("SendVideoButton"));

	if (SendVideoButton)
	{
		SendVideoButton->OnClicked.AddDynamic(this,&UInteractionWidget::OnSendVideoButtonClicked);
		SendVideoButton->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("SendVoiceButton not found!"));
	
	}

	

	UTextBlock* MessageTextBlock = Cast<UTextBlock>(GetWidgetFromName("MessageTextBlock"));
	if (MessageTextBlock)
	{
		MessageTextBlock->SetText(FText::FromString(""));
		MessageTextBlock->SetVisibility(ESlateVisibility::Collapsed);
	}

	bIsMessageShowing = false;

	UButton* TextMessageButton = Cast<UButton>(GetWidgetFromName("TextMessageButton"));
	if (TextMessageButton)
	{
		TextMessageButton->SetVisibility(ESlateVisibility::Hidden);
		TextMessageButton->OnClicked.AddDynamic(this, &UInteractionWidget::OnSendTextButtonClicked);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("TextMessageButton not found!"));
	
	}
	
	UButton* EyeTrackingButton = Cast<UButton>(GetWidgetFromName("EyeTrackingButton"));
	if (EyeTrackingButton)
	{
		EyeTrackingButton->SetVisibility(ESlateVisibility::Hidden);
		EyeTrackingButton->OnClicked.AddDynamic(this, &UInteractionWidget::OnSendTextButtonClicked);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("TextMessageButton not found!"));
	
	}

	
}

void UInteractionWidget::OnInteractionButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("InteractionButton clicked!"));
	if (Officer)
	{
		Officer->OnInteractionButtonClicked();
		SwitchInteractingButtonVisibility(true);
		CommunicateState = ECommunicateState::PrepareInteract;
		SwitchInteractionMethodButtonsVisibility(true);
		
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Officer is nullptr!"));
	}
}

void UInteractionWidget::OnEndInteractionButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("EndInteractionButton clicked!"));
	if (Officer)
	{
		Officer->OnEndInteractionButtonClicked();
		Officer->bIsTalking = false;
		SwitchInteractingButtonVisibility(false);
		SwitchInteractionMethodButtonsVisibility(false);
		UTextBlock* MessageTextBlock = Cast<UTextBlock>(GetWidgetFromName("MessageTextBlock"));
		if (MessageTextBlock)
		{
			MessageTextBlock->SetVisibility(ESlateVisibility::Hidden);
		}
		CommunicateState = ECommunicateState::Free;
	}

	
}

void UInteractionWidget::OnSendTextButtonClicked()
{
	ShowDevelopingMessage();
}

void UInteractionWidget::SwitchInteractingButtonVisibility(bool bIsInteracting)
{
	if (bIsInteracting)
	{
		// Hide InteractionButton and show EndInteractionButton
		UButton* InteractionButton = Cast<UButton>(GetWidgetFromName(TEXT("InteractionButton")));
		UButton* EndInteractionButton = Cast<UButton>(GetWidgetFromName(TEXT("EndInteractionButton")));
		if (InteractionButton && EndInteractionButton)
		{
			InteractionButton->SetVisibility(ESlateVisibility::Hidden);
			EndInteractionButton->SetVisibility(ESlateVisibility::Visible);
		}
	}
	else
	{
		// Hide EndInteractionButton and show InteractionButton
		UButton* InteractionButton = Cast<UButton>(GetWidgetFromName(TEXT("InteractionButton")));
		UButton* EndInteractionButton = Cast<UButton>(GetWidgetFromName(TEXT("EndInteractionButton")));
		if (InteractionButton && EndInteractionButton)
		{
			EndInteractionButton->SetVisibility(ESlateVisibility::Hidden);
			InteractionButton->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void UInteractionWidget::ShowDevelopingMessage()
{
	if (bIsMessageShowing)
	{
		return;
	}

	bIsMessageShowing = true;

	UTextBlock* MessageTextBlock = Cast<UTextBlock>(GetWidgetFromName("MessageTextBlock"));
	if (MessageTextBlock)
	{
		MessageTextBlock->SetText(FText::FromString("Feature in development, please stay tuned."));
		MessageTextBlock->SetVisibility(ESlateVisibility::Visible);
	}

	// Set a timer to hide the message after 2 seconds
	GetWorld()->GetTimerManager().SetTimer(MessageTimerHandle, this, &UInteractionWidget::HideDevelopingMessage, 2.0f, false);
}

void UInteractionWidget::HideDevelopingMessage()
{
	UTextBlock* MessageTextBlock = Cast<UTextBlock>(GetWidgetFromName("MessageTextBlock"));
	if (MessageTextBlock)
	{
		MessageTextBlock->SetText(FText::FromString(""));
		MessageTextBlock->SetVisibility(ESlateVisibility::Collapsed);
	}

	bIsMessageShowing = false;
}

void UInteractionWidget::SwitchInteractionMethodButtonsVisibility(bool bIsVisible)
{
	if (bIsVisible)
	{
		UTextBlock* MessageTextBlock = Cast<UTextBlock>(GetWidgetFromName("MessageTextBlock"));
		if (MessageTextBlock)
		{
			MessageTextBlock->SetVisibility(ESlateVisibility::Hidden);
		}
		
		UButton* VoiceMessageButton = Cast<UButton>(GetWidgetFromName("VoiceMessageButton"));

		if (VoiceMessageButton)
		{
			VoiceMessageButton->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("VoiceMessageButton not found!"));
	
		}

		UButton* VideoMessageButton = Cast<UButton>(GetWidgetFromName("VideoMessageButton"));

		if (VideoMessageButton)
		{
			VideoMessageButton->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("VoiceMessageButton not found!"));
	
		}
		UButton* TextMessageButton = Cast<UButton>(GetWidgetFromName("TextMessageButton"));
		if (TextMessageButton)
		{
			TextMessageButton->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("TextMessageButton not found!"));
	
		}

		UButton* EyeTrackingButton = Cast<UButton>(GetWidgetFromName("EyeTrackingButton"));
		if (EyeTrackingButton)
		{
			EyeTrackingButton->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("TextMessageButton not found!"));
	
		}
	}
	else
	{
		UButton* VoiceMessageButton = Cast<UButton>(GetWidgetFromName("VoiceMessageButton"));

		if (VoiceMessageButton)
		{
			VoiceMessageButton->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("VoiceMessageButton not found!"));
	
		}

		UButton* VideoMessageButton = Cast<UButton>(GetWidgetFromName("VideoMessageButton"));

		if (VideoMessageButton)
		{
			VideoMessageButton->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("VoiceMessageButton not found!"));
	
		}

		UButton* TextMessageButton = Cast<UButton>(GetWidgetFromName("TextMessageButton"));
		if (TextMessageButton)
		{
			TextMessageButton->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("TextMessageButton not found!"));
	
		}

		UButton* EyeTrackingButton = Cast<UButton>(GetWidgetFromName("EyeTrackingButton"));
		if (EyeTrackingButton)
		{
			EyeTrackingButton->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("TextMessageButton not found!"));
	
		}
	}
}



void UInteractionWidget::OnVoiceMessageButtonClicked()
{
	//检查是否能找到按钮
	UButton* SendVoiceButton = Cast<UButton>(GetWidgetFromName("SendVoiceButton"));

	if (SendVoiceButton)
	{
		SendVoiceButton->SetVisibility(ESlateVisibility::Visible);
		SwitchInteractionMethodButtonsVisibility(false);
		CommunicateState = ECommunicateState::VoiceRecording;
		UButton* EndInteractionButton = Cast<UButton>(GetWidgetFromName(TEXT("EndInteractionButton")));
		if (EndInteractionButton)
		{
			EndInteractionButton->SetVisibility(ESlateVisibility::Hidden);
			Officer->StartRecording();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("EndInteractionButton not found!"));
		}

		UTextBlock* MessageTextBlock = Cast<UTextBlock>(GetWidgetFromName("MessageTextBlock"));
		if (MessageTextBlock)
		{
			MessageTextBlock->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			MessageTextBlock->SetText(FText::FromString("Recording..."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("SendVoiceButton not found!"));
	
	}


}

void UInteractionWidget::OnVideoMessageButtonCLicked()
{
	ShowDevelopingMessage();
}

void UInteractionWidget::OnSendVoiceButtonClicked()
{
	//切换状态到waitingforresponse
	CommunicateState = ECommunicateState::WaitingForResponse;
	UButton* SendVoiceButton = Cast<UButton>(GetWidgetFromName("SendVoiceButton"));

	if (SendVoiceButton)
	{
		SendVoiceButton->SetVisibility(ESlateVisibility::Hidden);

		// 显示提示
		UTextBlock* MessageTextBlock = Cast<UTextBlock>(GetWidgetFromName("MessageTextBlock"));
		if (MessageTextBlock)
		{
			MessageTextBlock->SetText(FText::FromString("Waiting for response.."));
			MessageTextBlock->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		SwitchInteractingButtonVisibility(true);
		Officer->EndRecording();
		
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("SendVoiceButton not found!"));
	
	}
}

void UInteractionWidget::OnSendVideoButtonClicked()
{
	ShowDevelopingMessage();
}



