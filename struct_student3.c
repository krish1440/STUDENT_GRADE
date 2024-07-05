#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SUBJECTS 10
#define MAX_STUDENTS 100


struct Marks {
    char subjects[MAX_SUBJECTS][50]; 
    int marks[MAX_SUBJECTS];
    int credits[MAX_SUBJECTS];
};


struct Student {
    int id;
    char name[50];
    int age;
    struct Marks marks; 
    float cgpa; 
};

void addStudent(struct Student students[], int numStudents, int numSubjects);
void updateStudent(struct Student students[], int numStudents, int numSubjects);
void deleteStudent(struct Student students[], int numStudents);
void displayAllStudents(struct Student students[], int numStudents, int numSubjects);
void saveDataToCSV(struct Student students[], int numStudents, int numSubjects);
float calculateCGPA(struct Marks marks, int numSubjects);

int main() {
    int numSubjects;
    printf("Enter the number of subjects: ");
    scanf("%d", &numSubjects);
    getchar(); 
    if (numSubjects <= 0 || numSubjects > MAX_SUBJECTS) {
        printf("Invalid number of subjects. Please enter a number between 1 and %d.\n", MAX_SUBJECTS);
        return 1;
    }

    struct Student students[MAX_STUDENTS];
    int numStudents = 0;
    int choice;

    
    FILE *csvFile = fopen("students.csv", "r");
    if (csvFile != NULL) {

        char header[256];
        fgets(header, sizeof(header), csvFile);

    
        while (fscanf(csvFile, "%d,%49[^,],%d,%f", &students[numStudents].id, students[numStudents].name, &students[numStudents].age, &students[numStudents].cgpa) == 4) {
          
            for (int i = 0; i < numSubjects; i++) {
                fscanf(csvFile, ",%d,%d", &students[numStudents].marks.marks[i], &students[numStudents].marks.credits[i]);
            }
            numStudents++;
        }

        fclose(csvFile);
    }

    do {
        printf("\nMenu:\n");
        printf("1. Add Student\n");
        printf("2. Update Student\n");
        printf("3. Delete Student\n");
        printf("4. Display All Students\n");
        printf("5. Save Data to CSV\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addStudent(students, numStudents, numSubjects);
                numStudents++;
                break;
            case 2:
                updateStudent(students, numStudents, numSubjects);
                break;
            case 3:
                deleteStudent(students, numStudents);
                numStudents--;
                break;
            case 4:
                displayAllStudents(students, numStudents, numSubjects);
                break;
            case 5:
                saveDataToCSV(students, numStudents, numSubjects);
                break;
            case 6:
                printf("Exiting program.\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 6);

    return 0;
}

void addStudent(struct Student students[], int numStudents, int numSubjects) {
    if (numStudents < MAX_STUDENTS) {
        struct Student newStudent;

        printf("Enter student ID: ");
        scanf("%d", &newStudent.id);
        getchar(); 

        printf("Enter student name: ");
        fgets(newStudent.name, sizeof(newStudent.name), stdin);
        newStudent.name[strcspn(newStudent.name, "\n")] = '\0';
        
        printf("Enter student age: ");
        scanf("%d", &newStudent.age);
        getchar(); 

       
        printf("Enter marks and credits for each subject:\n");
        for (int i = 0; i < numSubjects; i++) {
            printf("Subject %d name: ", i+1);
            fgets(newStudent.marks.subjects[i], sizeof(newStudent.marks.subjects[i]), stdin);
            newStudent.marks.subjects[i][strcspn(newStudent.marks.subjects[i], "\n")] = '\0'; 
            
            printf("Marks for %s: ", newStudent.marks.subjects[i]);
            scanf("%d", &newStudent.marks.marks[i]);
            getchar(); 

            printf("Credits for %s: ", newStudent.marks.subjects[i]);
            scanf("%d", &newStudent.marks.credits[i]);
            getchar(); 
        }


        newStudent.cgpa = calculateCGPA(newStudent.marks, numSubjects);

        students[numStudents] = newStudent;

        printf("Student added successfully.\n");
    } else {
        printf("Maximum number of students reached.\n");
    }
}

void updateStudent(struct Student students[], int numStudents, int numSubjects) {
    int id, found = 0;

    printf("Enter the ID of the student to update: ");
    scanf("%d", &id);
    getchar(); 

    for (int i = 0; i < numStudents; i++) {
        if (students[i].id == id) {
            printf("Enter updated information for student %d:\n", id);
            printf("Enter student name: ");
            fgets(students[i].name, sizeof(students[i].name), stdin);
            students[i].name[strcspn(students[i].name, "\n")] = '\0';
            
            printf("Enter student age: ");
            scanf("%d", &students[i].age);
            getchar(); 

            printf("Enter updated marks and credits for each subject:\n");
            for (int j = 0; j < numSubjects; j++) {
                 printf("Subject: %s\n", students[i].marks.subjects[j]);
                printf("Marks for %s: ",students[i].marks.subjects[j]);
                scanf("%d", &students[i].marks.marks[j]);
                getchar(); 
                printf("Credits for %s: ", students[i].marks.subjects[j]);
                scanf("%d", &students[i].marks.credits[j]);
                getchar(); 
            }

            students[i].cgpa = calculateCGPA(students[i].marks, numSubjects);

            printf("Student updated successfully.\n");
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Student with ID %d not found.\n", id);
    }
}


void deleteStudent(struct Student students[], int numStudents) {
    int id, found = 0;

    printf("Enter the ID of the student to delete: ");
    scanf("%d", &id);

    for (int i = 0; i < numStudents; i++) {
        if (students[i].id == id) {
            for (int j = i; j < numStudents - 1; j++) {
                students[j] = students[j + 1];
            }
            printf("Student with ID %d deleted successfully.\n", id);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Student with ID %d not found.\n", id);
    }
}

void displayAllStudents(struct Student students[], int numStudents, int numSubjects) {
    if (numStudents > 0) {
        printf("\nAll Students:\n");
        printf("ID,Name,Age,CGPA");
        for (int i = 0; i < numSubjects; i++) {
            printf(",%s Marks,%s Credits", students[0].marks.subjects[i], students[0].marks.subjects[i]);
        }
        printf("\n");

      
        for (int i = 0; i < numStudents; i++) {
            printf("%d,%s,%d,%.2f", students[i].id, students[i].name, students[i].age, students[i].cgpa);
            for (int j = 0; j < numSubjects; j++) {
                printf(",%d,%d", students[i].marks.marks[j], students[i].marks.credits[j]);
            }
            printf("\n");
        }
    } else {
        printf("No students to display.\n");
    }
}

void saveDataToCSV(struct Student students[], int numStudents, int numSubjects) {
    FILE *file = fopen("students.csv","a"); 

    if (file != NULL) {
       
        long pos = ftell(file);
        if (pos == 0) {
            
            fprintf(file, "ID,Name,Age,CGPA");
            for (int i = 0; i < numSubjects; i++) {
                fprintf(file, ",%s Marks,%s Credits", students[0].marks.subjects[i], students[0].marks.subjects[i]);
            }
            fprintf(file, "\n");
        }
  
        for (int i = 0; i < numStudents; i++) {
            fprintf(file, "%d,%s,%d,%.2f", students[i].id, students[i].name, students[i].age, students[i].cgpa);
            for (int j = 0; j < numSubjects; j++) {
                fprintf(file, ",%d,%d", students[i].marks.marks[j], students[i].marks.credits[j]);
            }
            fprintf(file, "\n");
        }

        fclose(file);
        printf("Student data appended to students.csv successfully.\n");
    } else {
        printf("Error opening file for writing.\n");
    }
}

float calculateCGPA(struct Marks marks, int numSubjects) {
    float totalGradePoints = 0.0;
    int totalCredits = 0;

    for (int i = 0; i < numSubjects; i++) {
        int marksInRange = marks.marks[i];
        float gradePoint = 0.0;

        if (marksInRange >= 90)
            gradePoint = 10.0;
        else if (marksInRange >= 80)
            gradePoint = 9.0;
        else if (marksInRange >= 70)
            gradePoint = 8.0;
        else if (marksInRange >= 60)
            gradePoint = 7.0;
        else if (marksInRange >= 50)
            gradePoint = 6.0;
        else if (marksInRange >= 40)
            gradePoint = 5.0;
        else if (marksInRange <40)
            gradePoint = 0;

        totalGradePoints += gradePoint * marks.credits[i];
        totalCredits += marks.credits[i];
    }

    float cgpa = 0.0;
    if (totalCredits != 0) {
        cgpa = (totalGradePoints*10) / (totalCredits); 
    }

    return cgpa;
}

